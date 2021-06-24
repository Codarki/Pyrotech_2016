#ifndef PT_CONFIG_NODE_H
#define PT_CONFIG_NODE_H

#include "text_file_position.h"
#include "path.h"
#include "std/list.h"

namespace pt {

/// Class that holds key value pairs in text and any number of child nodes.
/// Any value can be parsed to any given type, given there is implementation
/// for the type. See parse_from_text() overloads for expanding.
/// Loading from XML files is also supported. In XML files attributes are
/// treated as child nodes.
///
/// Following example loads a file "some_file.xml". Then it searches a root
/// element "my_xml_element". Then it searches child element
/// "my_child_element" from the root element, and then tries to parse yet
/// again child element "my_value" into some_value. If the "my_value"
/// element is not found, sets some_value to 0.0f.
/// \code
/// config_node config("some_file.xml");
/// config_node::const_iterator i = config.find_child("my_xml_element");
/// assert(i != config.end());
/// 
/// float some_value;
/// i->child("my_child_element")->parse_optional("my_value", some_value, 0.0f);
/// \endcode
class config_node
{
private: // type definitions
    typedef std::list<config_node> container;
public:
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;

public:
    config_node();
    explicit config_node(std::string const& key, std::string const& value);
    /// Constructs config_node from \a file.
    explicit config_node(pt::path const& file);

    /// Getter for child node. \return \c iterator to the child node with a
    /// \a name. \throw If not found.
    const_iterator child(std::string const& name) const;
    /// \overload
    iterator child(std::string const& name);
    /// Getter for child node. \returns \c iterator to the child node with a
    /// \a name. \return end() \c iterator if not found.
    const_iterator find_child(std::string const& name) const;
    /// \overload
    iterator find_child(std::string const& name);

    /// \return \c iterator for the first child node.
    iterator begin();
    /// \return \c const_iterator for the first child node.
    const_iterator begin() const;

    /// \return end \c iterator.
    iterator end();
    /// \return end \c const_iterator.
    const_iterator end() const;

    /// Inserts \a node as a child. \return \c iterator to the added child.
    iterator insert_child(config_node const& node);

    /// \return Key of this node.
    std::string const& key() const;
    /// Sets key for this node.
    void set_key(std::string const& key);

    /// \return Value of this node parsed into \c T. \throw If cast failed.
    /// \throw If empty value.
    //template<typename T>
    //T value() const;
    /// Sets this nodes value to \a value.
    void set_value(std::string const& value);
    std::string const& value() const;

    /// Parses the value of this node into \a t. \throw If parse failed.
    template<typename T>
    void parse(T& t) const;

    /// Parses value of child \a name into \a result. \throw If not found.
    /// \throw If parse failed.
    template<typename T>
    void parse_child(std::string const& name, T& result) const;

    /// Parses value of child \a name into \a result. If no such child found,
    /// does nothing.
    template<typename T>
    void parse_optional_child(std::string const& name, T& result) const;
    /// Parses value of child \a name into \a result. If no such child found,
    /// sets \a result to a \a default_value.
    template<typename T>
    void parse_optional_child(std::string const& name, T& result,
        T const& default_value) const;

    /// \return Position of this node in text file.
    text_file_position const& position() const;
    /// Sets the position of this node in text file.
    void set_position(text_file_position const& position);

private: // data members
    std::string m_key;
    std::string m_value;
    container m_childs;
    text_file_position m_text_position;
};

std::string optional_child_value_text(config_node const& config,
    std::string const& child, std::string const& default_value_text);

/// Inserts new child node of \a key - \a value pair into \a config.
/// \relatesalso config_node
void write_child(config_node& config, std::string const& key,
    std::string const& value);
/// \overload
/// \relatesalso config_node
void write_child(config_node& config, std::string const& key, bool value);
/// \overload
/// \relatesalso config_node
void write_child(config_node& config, std::string const& key, int value);
/// \overload
/// \relatesalso config_node
void write_child(config_node& config, std::string const& key, float value);
/// \overload
/// \relatesalso config_node
void write_child(config_node& config, std::string const& key, double value);
/// \overload
/// \relatesalso config_node
void write_child(config_node& config, std::string const& key, char value);
/// \overload
/// \relatesalso config_node
void write_child(config_node& config, std::string const& key, size_t value);

/// Parses \a text into \a value.
/// \relatesalso config_node
void parse_from_text(bool& value, std::string const& text);
/// \overload
/// \relatesalso config_node
void parse_from_text(int& value, std::string const& text);
/// \overload
/// \relatesalso config_node
void parse_from_text(float& value, std::string const& text);
/// \overload
/// \relatesalso config_node
void parse_from_text(double& value, std::string const& text);
/// \overload
/// \relatesalso config_node
void parse_from_text(char& value, std::string const& text);
/// \overload
/// \relatesalso config_node
void parse_from_text(std::string& value, std::string const& text);
/// \overload
/// \relatesalso config_node
void parse_from_text(size_t& value, std::string const& text);

} // namespace pt

#include "config_node.inl"

#endif
