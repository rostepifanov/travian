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

    template <const std::string& type>
    class iterator
    {
        rapidxml::node_iterator<char> it;
        iterator(const html& html) : it (html.page.first_node(type.c_str())) { }

        bool has(const std::string& attribute)
        {
            return it->first_attribute(attribute.c_str());
        }

        std::string value(const std::string& attribute, unsigned order = 0)
        {
            rapidxml::xml_attribute<char> * node = it->first_attribute(attribute.c_str());

            while (order-- && node->next_attribute())
              node->next_attribute();

            return node->value();
        }

        iterator& operator++() { ++it; return *this; }



            //            node = html.first_node(type);
//                // Iterate over the brewerys
//                for (xml_node<> * brewery_node = root_node->first_node("Brewery"); brewery_node; brewery_node = brewery_node->next_sibling())
//                {
//                    printf("I have visited %s in %s. ",
//                        brewery_node->first_attribute("name")->value(),
//                        brewery_node->first_attribute("location")->value());
//                        // Interate over the beers
//                    for(xml_node<> * beer_node = brewery_node->first_node("Beer"); beer_node; beer_node = beer_node->next_sibling())
//                    {
//                        printf("On %s, I tried their %s which is a %s. ",
//                            beer_node->("dateSampled")->value(),
//                            beer_node->first_attribute("name")->value(),
//                            beer_node->first_attribute("description")->value());
//                        printf("I gave it the following review: %s", beer_node->value());
//                    }

//                }


        bool has(const std::string& attribute)
        {

        }
    };
};

#endif // HTML_H
