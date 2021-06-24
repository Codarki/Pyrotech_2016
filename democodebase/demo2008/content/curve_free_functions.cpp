#include "curve_free_functions.h"
#include "pt/exceptions/text_file_error.h"
#include "pt/animatable_value.h"
#include "pt/any_error.h"
#include "pt/config_node.h"
#include "pt/parameter_node.h"
#include "pt/parameter_collection.h"
#include "pt/vector_utility.h"
#include "pt/math/cr_spline.h"
//#include "pt/math/cr_data_set.h"
//#include "pt/math/cubic_hermite_spline.h"
//#include "pt/math/spline_value_source.h"
#include "pt/math/step_curve.h"
#include "pt/format.h"
#include "pt/string_wrapper.h"
#include "pt/std/string.h"

// random
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

namespace content {

namespace {
    typedef boost::mt19937 random_generator_type;
    typedef boost::uniform_real<float> distribution_type;
    typedef boost::variate_generator<random_generator_type&, distribution_type>
        gen_type;

    random_generator_type generator(42u);
    gen_type random_gen(generator, distribution_type(0, 1));
} // anonymous

template<typename T, int dim>
pt::vector<T,dim> random_value(
    pt::vector<T,dim> const& min_value, pt::vector<T,dim> const& max_value,
    bool uniform)
{
    pt::vector<T,dim> range = max_value - min_value;
    pt::vector<T,dim> result = min_value;

    pt::vector<float,dim> random_coeff;
    
    random_coeff[0] = random_gen();
    if (uniform)
    {
        for (int i = 1; i < dim; ++i)
            random_coeff[i] = random_coeff[0];
    }
    else
    {
        for (int i = 1; i < dim; ++i)
            random_coeff[i] = random_gen();
    }

    for (int i = 0; i < dim; ++i)
    {
        result[i] += range[i] * random_coeff[i];
    }

    return result;
}

float random_value(float min_value, float max_value, bool)
{
    float range = max_value - min_value;
    float result = min_value + range * random_gen();

    return result;
}

template<typename T>
class random_keys
{
public:
    random_keys(config_node const& random_config)
    {
        assert(random_config.key() == "random");

        int seed = 42;
        random_config.parse_child("seed", seed);

        generator.seed(boost::uint32_t(seed));

        float start_time = 0;
        float end_time = 0;
        random_config.child("timing")->parse_child("start_time", start_time);
        random_config.child("timing")->parse_child("end_time", end_time);

        float min_step = 0;
        float max_step = 0;

        config_node::const_iterator interval_config
            = random_config.find_child("random_interval");
        if (interval_config != random_config.end())
        {
            interval_config->parse_child("min_step", min_step);
            interval_config->parse_child("max_step", max_step);
        }
        else
        {
            interval_config = random_config.find_child("uniform_interval");
            if (interval_config == random_config.end())
            {
                throw exceptions::text_file_error(random_config.position(),
                    format("random element requires child element "
                    "\"random_interval\" or \"uniform_interval\""));
            }

            interval_config->parse_child("step", min_step);
            max_step = min_step;
        }

        if (min_step == 0)
            min_step = 0.00001f;

        T min_value;
        T max_value;
        bool uniform_random_value = true;
        bool additive_value = false;

        random_config.child("value")->parse_child("min", min_value);
        random_config.child("value")->parse_child("max", max_value);
        random_config.child("value")->parse_optional_child("uniform", uniform_random_value);
        random_config.child("value")->parse_optional_child("additive", additive_value);

        // assignment to remove warning (potentially uninitialized variable)
        T start_value = min_value;
        if (additive_value)
            random_config.child("value")->parse_child("start", start_value);

        for (float time = start_time; time < end_time;)
        {
            T value
                = random_value(min_value, max_value, uniform_random_value);

            float time_step = min_step + random_gen() * (max_step - min_step);
            assert(time_step > 0 && time_step <= max_step);

            if (additive_value)
            {
                start_value += value;
                value = start_value;
            }

            //log(str(format("random key time: %1%  value: %2%")
            //    % time % pt::to_text(value)));

            m_keys.push_back(std::make_pair(time, value));

            time += time_step;
        }
    }

    std::vector<std::pair<float,T> > const& key_value_pairs()
    {
        return m_keys;
    }

private: // data members
    std::vector<std::pair<float,T> > m_keys;
};

template<>
random_keys<std::string>::random_keys(config_node const& random_config)
{
    assert(random_config.key() == "random");

    int seed = 42;
    random_config.parse_child("seed", seed);

    generator.seed(boost::uint32_t(seed));

    float start_time = 0;
    float end_time = 0;
    random_config.child("timing")->parse_child("start_time", start_time);
    random_config.child("timing")->parse_child("end_time", end_time);

    float min_step = 0;
    float max_step = 0;

    config_node::const_iterator interval_config
        = random_config.find_child("random_interval");
    if (interval_config != random_config.end())
    {
        interval_config->parse_child("min_step", min_step);
        interval_config->parse_child("max_step", max_step);
    }
    else
    {
        interval_config = random_config.find_child("uniform_interval");
        if (interval_config == random_config.end())
        {
            throw exceptions::text_file_error(random_config.position(),
                format("random element requires child element "
                "\"random_interval\" or \"uniform_interval\""));
        }

        interval_config->parse_child("step", min_step);
        max_step = min_step;
    }

    std::vector<std::string> values;
    config_node::const_iterator values_config
        = random_config.child("values");
    if (interval_config != random_config.end())
    {
        for (auto it = values_config->begin(), it_end = values_config->end();
            it != it_end; ++it)
        {
            assert(it->key() == "value");
            values.push_back(it->value());
        }
    }

    if (min_step == 0)
        min_step = 0.00001f;

    for (float time = start_time; time < end_time;)
    {
        int random_index = static_cast<int>(
            random_value(0.0f, static_cast<float>(values.size()) - 0.01f, true));

        float time_step = min_step + random_gen() * (max_step - min_step);
        assert(time_step > 0 && time_step <= max_step);

        m_keys.push_back(std::make_pair(time, values[random_index]));

        time += time_step;
    }
}

template<typename T>
std::shared_ptr<animatable_value_source<T> > create_spline(
    config_node const& config, std::string& name);

math::cr_data_set<float> create_data_set_float(config_node const& config,
    std::string& name, float parent_time_offset = 0.0f, float parent_time_multiplier = 1.0f)
{
    config.parse_child("name", name);

    math::data_set<float> values;

    float time_offset = 0.0f;
    float time_multiplier = 1.0f;
    float value_multiplier = 1.0f;
    float value_offset = 0.0f;
    config.parse_optional_child("time_offset", time_offset);
    config.parse_optional_child("time_multiplier", time_multiplier);
    config.parse_optional_child("value_multiplier", value_multiplier);
    config.parse_optional_child("value_offset", value_offset);

    for (config_node::const_iterator j = config.begin();
        j != config.end(); ++j)
    {
        if (j->key() == "key")
        {
            float time;
            float value;

            j->parse_child("time", time);
            j->parse_child("value", value);

            values.add(
                time * (time_multiplier * parent_time_multiplier) + (time_offset + parent_time_offset),
                value * value_multiplier + value_offset);
        }
        else if (j->key() == "random")
        {
            random_keys<float> keys(*j);

            typedef std::vector<std::pair<float, float> > container;
            for (container::const_iterator key_iter = keys.key_value_pairs().begin();
                key_iter != keys.key_value_pairs().end(); ++key_iter)
            {
                float time = key_iter->first;
                float value = key_iter->second;

                values.add(
                    time * time_multiplier + time_offset,
                    value * value_multiplier + value_offset);
            }
        }
    }

    math::cr_data_set<float> data(values);
    return data;
}

template<>
std::shared_ptr<animatable_value_source<float> > create_spline(
    config_node const& config, std::string& name)
{
    math::cr_splinef spline(create_data_set_float(config, name));

    return std::shared_ptr<animatable_value_source<float> >(
        new math::cr_splinef_value_source(spline));
}

template<int C>
std::shared_ptr<animatable_value_source<vector<float,C> > > create_vector_spline(
    config_node const& config, std::string& name)
{
    config.parse_child("name", name);

    auto iter1 = config.find_child("catmull_rom");
    if (iter1 != config.end())
    {
        // Curve editor exported curve.

        float time_offset = 0.0f;
        float time_multiplier = 1.0f;
        config.parse_optional_child("time_offset", time_offset);
        config.parse_optional_child("time_multiplier", time_multiplier);

        math::cr_data_set<float> data[C];

        int count = 0;
        for (auto const& child : config)
        {
            if (child.key() == "catmull_rom")
            {
                std::string child_name;
                child.parse_child("name", child_name);

                int index = count;
                child.parse_optional_child("index", index);

                if (index >= C)
                    throw any_error(pt::format("Too many curves in '%1%', child name '%2%'") % name % child_name);

                if (data[index].size() > 0)
                    throw any_error(pt::format("Overwriting '%1%' index %2%, child name '%3%'") % name % index % child_name);

                data[index] = create_data_set_float(child, child_name, time_offset, time_multiplier);
                ++count;
            }
        }

        math::cr_spline<float, C> spline(data);

        return std::shared_ptr<animatable_value_source<vector<float, C> > >(
            new math::spline_value_source<math::cr_spline<float, C> >(spline));
    }

    // Old style curve.
    math::data_set<float> values[C];

    float time_offset = 0.0f;
    float time_multiplier = 1.0f;

    pt::vector<float,C> value_multiplier;
    pt::vector<float,C> value_offset;
    for (size_t i = 0; i < C; ++i)
    {
        value_multiplier[i] = 1.0f;
        value_offset[i] = 0.0f;
    }

    config.parse_optional_child("time_offset", time_offset);
    config.parse_optional_child("time_multiplier", time_multiplier);
    config.parse_optional_child("value_multiplier", value_multiplier);
    config.parse_optional_child("value_offset", value_offset);

    for (config_node::const_iterator j = config.begin();
        j != config.end(); ++j)
    {
        if (j->key() == "key")
        {
            float time;
            vector<float,C> value;

            j->parse_child("time", time);
            j->parse_child("value", value);

            for (size_t k = 0; k < C; ++k)
            {
                values[k].add(
                    time * time_multiplier + time_offset,
                    value[k] * value_multiplier[k] + value_offset[k]);
            }
        }
        else if (j->key() == "random")
        {
            random_keys<pt::vector<float,C> > keys(*j);

            typedef std::vector<std::pair<float,pt::vector<float,C> > > container;
            for (container::const_iterator key_iter = keys.key_value_pairs().begin();
                key_iter != keys.key_value_pairs().end(); ++key_iter)
            {
                float time = key_iter->first;
                vector<float,C> value = key_iter->second;

                for (size_t k = 0; k < C; ++k)
                {
                    values[k].add(
                        time * time_multiplier + time_offset,
                        value[k] * value_multiplier[k] + value_offset[k]);
                }
            }
        }
    }

    math::cr_data_set<float> data[C];
    for (size_t i = 0; i < C; ++i)
        data[i] = math::cr_data_set<float>(values[i]);

    math::cr_spline<float,C> spline(data);

    return std::shared_ptr<animatable_value_source<vector<float,C> > >(
        new math::spline_value_source<math::cr_spline<float,C> >(spline));
}

std::shared_ptr<animatable_value_source<std::string> > create_step_curve(
    config_node const& config, std::string& name)
{
    config.parse_child("name", name);

    float time_offset = 0.0f;
    float time_multiplier = 1.0f;
    config.parse_optional_child("time_offset", time_offset);
    config.parse_optional_child("time_multiplier", time_multiplier);

    math::data_set<std::string> values;

    for (config_node::const_iterator j = config.begin();
        j != config.end(); ++j)
    {
        if (j->key() == "key")
        {
            float time;
            std::string value;

            j->parse_child("time", time);
            j->parse_child("value", value);

            values.add(time * time_multiplier + time_offset, value);
        }
    }

    return std::shared_ptr<animatable_value_source<std::string> >(
        new math::step_curve_string_value_source(values));
}

void append_dataset(
    math::data_set<std::string>& target,
    std::shared_ptr<parameter_node> const& node,
    float starting_time,
    std::string const& value)
{
    std::shared_ptr<animatable_value_base> step_curve_base = node->value();

    std::shared_ptr<animatable_value<std::string> > step_curve_animatable_value
        = std::dynamic_pointer_cast<animatable_value<std::string>>(step_curve_base);

    if (!step_curve_animatable_value)
    {
        throw pt::any_error(pt::format("%1% is not and text step curve") % value);
    }

    std::shared_ptr<math::step_curve_string_value_source> step_curve_value_source
        = std::dynamic_pointer_cast<math::step_curve_string_value_source>(step_curve_animatable_value->source());

    if (!step_curve_value_source)
    {
        throw pt::any_error(pt::format("%1% is not and text step curve") % value);
    }

    pt::math::step_curve_string const& step_curve_string = step_curve_value_source->step_curve();
    pt::math::data_set<std::string> const& data_set = step_curve_string.dataset();

    // Append all data to this curve
    for (auto k = data_set.begin(), k_end = data_set.end(); k != k_end; ++k)
    {
        target.add(starting_time + k->time(), k->value());
    }

}

std::shared_ptr<animatable_value_source<std::string> > create_reference_step_curve(
    config_node const& config, std::string& name, parameter_collection& curves)
{
    config.parse_child("name", name);

    float time_offset = 0.0f;
    float time_multiplier = 1.0f;
    config.parse_optional_child("time_offset", time_offset);
    config.parse_optional_child("time_multiplier", time_multiplier);

    math::data_set<std::string> values;

    for (config_node::const_iterator j = config.begin();
        j != config.end(); ++j)
    {
        if (j->key() == "key")
        {
            float time;
            std::string value;

            j->parse_child("time", time);

            float starting_time = time * time_multiplier + time_offset;

            // Value refers to another curve
            j->parse_child("value", value);

            // Extract dataset from existing curve
            pt::parameter_collection::const_iterator curve_iterator = curves.child(value);

            append_dataset(
                values,
                *curve_iterator,
                starting_time,
                value);
        }
        else if (j->key() == "random")
        {
            random_keys<std::string> keys(*j);

            typedef std::vector<std::pair<float,std::string> > container;
            for (container::const_iterator key_iter = keys.key_value_pairs().begin();
                key_iter != keys.key_value_pairs().end(); ++key_iter)
            {
                float time = key_iter->first;
                float starting_time = time * time_multiplier + time_offset;

                std::string value = key_iter->second;

                pt::parameter_collection::const_iterator curve_iterator = curves.child(value);

                append_dataset(
                    values,
                    *curve_iterator,
                    starting_time,
                    value);
            }
        }
    }

    return std::shared_ptr<animatable_value_source<std::string> >(
        new math::step_curve_string_value_source(values));
}

std::shared_ptr<parameter_node> create_spline(config_node const& config)
{
    std::string name;
    std::shared_ptr<animatable_value_base> value;

    if (config.key() == "catmull_rom")
    {
        value.reset(
            new animatable_value<float>(create_spline<float>(config, name)));
    }
    else if (config.key() == "catmull_rom2")
    {
        value.reset(new animatable_value<vector2f>(
            create_vector_spline<2>(config, name)));
    }
    else if (config.key() == "catmull_rom3")
    {
        value.reset(new animatable_value<vector3f>(
            create_vector_spline<3>(config, name)));
    }
    else if (config.key() == "catmull_rom4")
    {
        value.reset(new animatable_value<vector4f>(
            create_vector_spline<4>(config, name)));
    }
    else if (config.key() == "step_curve")
    {
        std::string type = config.child("type")->value();
        if (type == "text")
        {
            std::shared_ptr<animatable_value_source<std::string> > source(
                create_step_curve(config, name));

            std::shared_ptr<animatable_value<std::string> > anim_value(
                new animatable_value<std::string>(source));

            value = anim_value;
        }
        else
        {
            throw any_error(
                format("step_curve with type \"%1%\" not implemented yet")
                % type);
        }
    }
    else if (config.key() == "reference_step_curve")
    {
        // skipping
    }
    else
    {
        throw any_error(config.key() + " is not a spline");
    }

    std::shared_ptr<parameter_node> result(new parameter_node(name, value));

    return result;
}

std::shared_ptr<parameter_node> create_optional_reference_curve(
    config_node const& config,
    parameter_collection& existing_curves)
{
    std::string name;
    std::shared_ptr<animatable_value_base> value;

    if (config.key() == "reference_step_curve")
    {
        std::string type = config.child("type")->value();
        if (type == "text")
        {
            std::shared_ptr<animatable_value_source<std::string> > source(
                create_reference_step_curve(config, name, existing_curves));

            std::shared_ptr<animatable_value<std::string> > anim_value(
                new animatable_value<std::string>(source));

            value = anim_value;
        }
        else
        {
            throw any_error(
                format("step_curve with type \"%1%\" not implemented yet")
                % type);
        }
    }
    else
    {
        return nullptr;
    }

    std::shared_ptr<parameter_node> result(new parameter_node(name, value));

    return result;
}

} // namespace content
