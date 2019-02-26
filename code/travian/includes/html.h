#ifndef HTML_H
#define HTML_H

#include "defs.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_iterators.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"

using namespace rapidxml;

class html
{
    rapidxml::xml_document<> page;
public:
    html(const std::string& content) { page.parse<0>(const_cast<char*>(content.c_str())); }

    template <const char* type>
    class iterator
    {public:
        rapidxml::node_iterator<char> it;
        iterator(xml_node<char>* ptr = nullptr) : it (ptr) { }
        iterator(const html& html) : it (html.page.first_node(type)) { }
    public:
        bool has_attribute(const std::string& attribute)
        {
            return it->first_attribute(attribute.c_str());
        }

        std::string get_attribute(const std::string& attribute, unsigned order = 0)
        {
            rapidxml::xml_attribute<char> * pointer = it->first_attribute(attribute.c_str());

            while (order-- && pointer->next_attribute())
              pointer->next_attribute();

            return pointer->value();
        }

        bool has_node(const std::string& node)
        {
            return it->first_node(node.c_str());
        }

        template <const char* node>
        iterator<node> get_node(unsigned order = 0)
        {
            rapidxml::xml_node<char> * pointer = it->first_node(node);

            while (order-- && pointer->next_sibling())
              pointer->next_sibling();

            return iterator<node>(pointer);
        }

        iterator& operator++ () { ++it; return *this; }

        bool operator== (const iterator& right) { return it == right.it; }

        bool operator!= (const iterator& right) { return it != right.it; }

        std::string operator* () const
        {
            assert(it->value());
            return std::string(it->value());
        }
//                for (xml_node<> * brewery_node = root_node->first_node("Brewery"); brewery_node; brewery_node = brewery_node->next_sibling())
//                        brewery_node->first_attribute("location")->value());
//                    for(xml_node<> * beer_node = brewery_node->first_node("Beer"); beer_node; beer_node = beer_node->next_sibling())
//                            beer_node->("dateSampled")->value(),
//                            beer_node->first_attribute("name")->value(),

//        bool has(const std::string& attribute)
//        {

//        }
    };

    template <const char* type>
    iterator<type> begin() { return iterator<type>(*this); }

    template <const char* type>
    iterator<type> end() { iterator<type> it; return iterator<type>(*this); }

};

#endif // HTML_H
