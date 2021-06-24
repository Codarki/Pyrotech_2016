#include "config_node_utility.h"
#include "file_input_stream.h"
#include "data_stream_content.h"
#include "any_error.h"
#include "config_node.h"
#include "log.h"
#include "format.h"
#include "std/memory.h"
#include "std/vector.h"

#pragma warning(push)
#pragma warning(disable: 4244) // possible loss of data
#define TIXML_USE_STL
#include "tinyxml/tinyxml.h"
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable: 4127) // conditional expression is constant

namespace pt {

// Flag whether we log all debug info from loading xml.
bool const xml_load_debug_output = false;
void parse_declaration(TiXmlDeclaration* declaration)
{
    assert(declaration);
    if (xml_load_debug_output)
    {
        log(format("declaration version %1% encoding %2%")
            % declaration->Version() % declaration->Encoding());
    }
}

// Returns a new node from xml element. All the attributes are child nodes
// to the node, unless the attribute name is "v".
std::shared_ptr<config_node> parse_element(text_file_position const& parent_position, TiXmlElement* element)
{
    assert(element);
    std::string const & key = element->ValueStr();
    //std::string text = element->GetText() ? element->GetText() : "";

    if (xml_load_debug_output)
        log(format("element \"%1%\"") % key);

    auto new_node = std::make_shared<config_node>();
    new_node->set_key(key);

    new_node->set_position(text_file_position(
        parent_position.path(),
        element->Row(), element->Column()));

    TiXmlAttribute* attribute = element->FirstAttribute();
    while(attribute)
    {
        logging::scoped_indent ident;
        std::string const& name = attribute->NameTStr();
        std::string const& value = attribute->ValueStr();

        if (name == "v")
        {
            new_node->set_value(value);
            new_node->set_position(text_file_position(
                parent_position.path(),
                attribute->Row(), attribute->Column()));

            if (xml_load_debug_output)
            {
                log(format("attribute \"%1%\" set as value \"%2%\"")
                    % name % value);
            }
        }
        else
        {
            config_node attribute_node;
            attribute_node.set_key(name);
            attribute_node.set_value(value);
            attribute_node.set_position(text_file_position(
                parent_position.path(),
                attribute->Row(), attribute->Column()));

            new_node->insert_child(attribute_node);

            if (xml_load_debug_output)
                log(format("attribute \"%1%\" = \"%2%\"") % name % value);
        }

        attribute = attribute->Next();
    }

    return new_node;
}

void parse_comment(TiXmlComment*)
{
    //assert(comment);
}

// Text in xml is considered as the value of the node.
void parse_text(config_node& parent, TiXmlText* text)
{
    assert(text);
    logging::scoped_indent ident;
    //std::string const& name = attribute->NameTStr();
    std::string const& value = text->ValueStr();

    parent.set_value(value);

    if (xml_load_debug_output)
        log(format("text \"%1%\"") % value);
}

void parse(config_node& parent, TiXmlNode* xml_node)
{
    logging::scoped_indent indent;

    TiXmlDocument* document = 0;
    TiXmlComment* comment = 0;
    TiXmlUnknown* unknown = 0;

	std::shared_ptr<config_node> child;

    switch(xml_node->Type())
    {
    case TiXmlNode::DOCUMENT:
        document = xml_node->ToDocument();
        log("document");
        break;
    case TiXmlNode::ELEMENT:
        child = parse_element(parent.position(), xml_node->ToElement());
        break;
    case TiXmlNode::COMMENT:
        comment = xml_node->ToComment();
        //log("comment");
        break;
    case TiXmlNode::UNKNOWN:
        unknown = xml_node->ToUnknown();
        log("unknown");
        break;
    case TiXmlNode::TEXT:
        parse_text(parent, xml_node->ToText());
        break;
    case TiXmlNode::DECLARATION:
        parse_declaration(xml_node->ToDeclaration());
        break;
    default:
        throw std::exception("invalid xml node");
    }

    config_node::iterator i = parent.end();
    if (child)
    {
        i = parent.insert_child(*child);
        i->set_position(text_file_position(
            parent.position().path(),
            xml_node->Row(), xml_node->Column()));
    }

    for (TiXmlNode* xml_child = xml_node->FirstChild(); xml_child;
        xml_child = xml_child->NextSibling())
    {
        if (child)
            parse(*i, xml_child);
        else
            parse(parent, xml_child);
    }
}

void config_node_builder_xml(config_node& node, pt::path const& xml)
{
    TiXmlDocument doc;
    //TiXmlDocument doc(xml.native_file_string());

    //log(format("Creating config_node tree from \"%1%\"")
    //    % xml.native_file_string());

    file_input_stream file(xml);
    data_stream_content data(file);

    char const* ptr = reinterpret_cast<char const*>(data.data());
    std::string text(ptr, data.size());
    assert(text.c_str()[data.size()] == 0);

    doc.Parse(text.c_str(), 0, TIXML_DEFAULT_ENCODING);

    if (doc.Error())
    {
        std::string text2 = (format("Couldn't load xml \"%1%\" at row %2% column %3%: %4%")
            % xml.string()
            % doc.ErrorRow()
            % doc.ErrorCol()
            % doc.ErrorDesc()).move_string();

        throw std::exception(text2.c_str());
    }


    //if (!doc.LoadFile())
    //{
    //    assert(doc.Error());
    //    std::string text = str(
    //        format("Couldn't load xml \"%1%\" at row %2% column %3%: %4%")
    //        % xml.native_file_string()
    //        % doc.ErrorRow()
    //        % doc.ErrorCol()
    //        % doc.ErrorDesc());

    //    throw std::exception(text.c_str());
    //}

    TiXmlHandle doc_handle(&doc);
    TiXmlHandle root_handle(0);

    node.set_position(
        text_file_position(xml.string(), doc.Row(), doc.Column()));

    //TiXmlElement const* root = doc.RootElement();

    for (TiXmlNode* child = doc.FirstChild(); child;
        child = child->NextSibling())
    {
        parse(node, child);
    }
}

bool is_recursion_depth_greater_than(config_node const& node, size_t count)
{
    for (config_node::const_iterator i = node.begin(); i != node.end(); ++i)
    {
        if (count == 0)
            return true;

        bool child_depth = is_recursion_depth_greater_than(*i, count - 1);
        if (child_depth)
            return true;
    }
    return false;
}

void write_node_recursively(config_node const& node, TiXmlElement* xml_node)
{
    for (config_node::const_iterator i = node.begin(); i != node.end(); ++i)
    {
        // If child depth is 0, add as attribute
        if (!is_recursion_depth_greater_than(*i, 0))
        {
            xml_node->SetAttribute(i->key(), i->value());
        }
        else
        {
            TiXmlElement* element = new TiXmlElement(i->key());
            write_node_recursively(*i, element);

            xml_node->LinkEndChild(element);
        }
    }
}

void create_xml_from_config(TiXmlDocument& doc, config_node const& node)
{
    doc.LinkEndChild(new TiXmlDeclaration("1.0", "", ""));

    TiXmlElement* element = new TiXmlElement(node.key());
    write_node_recursively(node, element);
    doc.LinkEndChild(element);
}

void write_xml(pt::path const& xml, config_node const& config)
{
    TiXmlDocument doc;
    create_xml_from_config(doc, config);

    doc.SaveFile(xml.string());
}

void write_to_text(std::string& text, config_node const& config)
{
    TiXmlDocument doc;
    create_xml_from_config(doc, config);

    TiXmlPrinter printer;
    printer.SetLineBreak("\r\n");

    doc.Accept(&printer);

    text = printer.Str();
}

void create_config_from_text(config_node& node, std::string const& text)
{
    TiXmlDocument doc;

    char const* result = doc.Parse(text.c_str());
    if (!result)
        throw any_error("XML parsing error");

    TiXmlHandle doc_handle(&doc);
    TiXmlHandle root_handle(0);

    node.set_position(text_file_position("unknown", doc.Row(), doc.Column()));

    for (TiXmlNode* child = doc.FirstChild(); child;
        child = child->NextSibling())
    {
        parse(node, child);
    }
}

} // namespace pt

#pragma warning(pop)

