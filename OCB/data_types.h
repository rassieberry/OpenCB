#pragma once

#include <vector>
#include <opencv2/core/mat.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <wtypes.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <fstream>
#include <iostream>
#include <cmath>

#include "utilities.h"

namespace ocb
{

    // so here we have a new data-type, the Thing.
    // honestly im not sure what i want to do with it
    /* i know that this is the "unit" of a thought
     * so everything that can be thought of, is a thing.
     * but i dont know what i do with them after creating them
     * i think i need to use the attraction and repulsion somehow
     * but im not sure how, either. for one thing, i need to be
     * able to calculate those, which will probably just be
     * found by a function similar to the evaluate() function
     * that i already have for collections. but since everything
     * needs to be a Thing, i need to rewrite that function so that
     * it applies to this new datatype and not just collections.
     *
     * also, just being attracted and repulsed by different things doesnt
     * necessarily create behavior. i still need the underlying data
     * types to actually get behavior to happen.
    */


    class Thing
    {
    public:
        int index;
        bool is_group;
        bool is_feeling;
        bool is_experience;
        bool is_action;
        bool is_event;
        bool is_collection;
        bool is_sequence;
        bool is_relation;
        int target_index;
        std::vector<int> things;

        double attraction = 0;
        double repulsion = 0;

        void set_index(int id)
        {
            index = id;
        }

        void set_values(double a, double r)
        {
            attraction = a;
            repulsion = r;
        }

        void add_thing(int id)
        {
            is_group = true;
            is_feeling = false;
            is_experience = false;
            is_action = false;
            is_event = false;
            is_collection = false;
            is_sequence = false;
            is_relation = false;
            things.push_back(id);
        }

        void set_feeling(int id)
        {
            is_group = false;
            is_feeling = true;
            is_experience = false;
            is_action = false;
            is_event = false;
            is_collection = false;
            is_sequence = false;
            is_relation = false;
            target_index = id;
        }

        void set_experience(int id)
        {
            is_group = false;
            is_feeling = false;
            is_experience = true;
            is_action = false;
            is_event = true;
            is_collection = false;
            is_sequence = false;
            is_relation = false;
            target_index = id;
        }

        void set_action(int id)
        {
            is_group = false;
            is_feeling = false;
            is_experience = false;
            is_action = true;
            is_event = true;
            is_collection = false;
            is_sequence = false;
            is_relation = false;
            target_index = id;
        }

        void set_event(int id)
        {
            is_group = false;
            is_feeling = false;
            is_experience = false;
            is_action = false;
            is_event = true;
            is_collection = false;
            is_sequence = false;
            is_relation = false;
            target_index = id;
        }

        void set_collection(int id)
        {
            is_group = false;
            is_feeling = false;
            is_experience = false;
            is_action = false;
            is_event = false;
            is_collection = true;
            is_sequence = false;
            is_relation = false;
            target_index = id;
        }

        void set_sequence(int id)
        {
            is_group = false;
            is_feeling = false;
            is_experience = false;
            is_action = false;
            is_event = false;
            is_collection = false;
            is_sequence = true;
            is_relation = false;
            target_index = id;
        }

        void set_relation(int id)
        {
            is_group = false;
            is_feeling = false;
            is_experience = false;
            is_action = false;
            is_event = false;
            is_collection = false;
            is_sequence = false;
            is_relation = true;
            target_index = id;
        }
    };

    // just a simple class that holds feelings
    // one of these is stored in every other type of data object
    // besides an action, which has to be associated with
    // an experience of a feeling
    class Feeling
    {
    public:
        int index = 0;
        double happiness = 0;
        double sadness = 0;
        double restlessness = 0;
        double curiosity = 0;
        double questioning = 0;
        double tiredness = 0;
        double tolerance = 0;

        void set_feelings(std::vector<double> feelings)
        {
            happiness = feelings[0];
            sadness = feelings[1];
            restlessness = feelings[2];
            curiosity = feelings[3];
            questioning = feelings[4];
            tiredness = feelings[5];
            tolerance = feelings[6];
        }

        void set_index(int id)
        {
            index = id;
        }
    };

    // currently only holds an image or a feeling
    // but eventually this will contain sounds as well
    // feelings are considered experiences, btw.
    class Experience
    {
    public:
        bool is_feeling = false;
        Feeling this_feel;
        int time = 0;
        std::vector<cv::Mat> this_image;

        void set_image(std::vector<cv::Mat> img)
        {
            is_feeling = false;
            this_image = img;
        }

        void set_feeling(Feeling f)
        {
            is_feeling = true;
            this_feel = f;
        }
    };

    // anything the Cantor system can do is considered an Action
    // action type is an integer. you can find which ones are which
    // in the Body.h file or in the documentation
    // add_thought() function isn't implemented yet
    class Action
    {
    public:
        bool is_thought = false;
        int act_type = 0;
        double value = 0;
        std::vector<int> collections_indices;

        void set_act_type(int i)
        {
            is_thought = false;
            act_type = i;
        }

        void add_thought(int index)
        {
            is_thought = true;
            collections_indices.push_back(index);
        }
    };

    // anything real that "happens" is considered an event.
    // these get aggregated together in a Collection
    // of course, events can have a feeling associated with them
    class Event
    {
    public:
        bool event_type = true;
        unsigned __int64 time = 0;
        int index = -1;

        Action act;
        Experience exp;
        Feeling fln;


        void set_action(Action a)
        {
            act = a;
            event_type = false;
        }

        void set_experience(Experience e)
        {
            exp = e;
            event_type = true;
        }

        void set_feeling(Feeling f)
        {
            fln = f;
        }

        void set_index(int id)
        {
            index = id;
        }
    };

    // this is the container class for pretty much every data type
    // in here. it represents a unit of memory in the mind. anything
    // which can be thought of is considered a Collection.

    // it's also the place where different values are put together
    // attraction - how much the system likes this collection
    // repulsion - how much it dislikes this collection
    // time - the average time the events in the collection occured at
    // value - the "value" of a collection is just how much it is worth to the system
    //      to try to make it happen
    // correlation - a measure of how close together in time the events occured at
    // similarity - a measure of how similar the images in the collection are to each other
    // importance - a measure of how often the collection shows up in Relations objects that
    //      show a high relation between the collections in them (more on that later)
    class Collection
    {
    public:
        double attraction;
        double repulsion;
        unsigned __int64 time;
        double value;
        double correlation;
        double similarity;
        double importance;
        int index;
        std::vector<Event> events_vector;
        std::vector<Event> experiences_vector;
        std::vector<Event> actions_vector;
        Feeling fln;
        Feeling fln_on_create;
        std::vector<int> collections;
        std::vector<int> relations;
        std::vector<int> events;
        std::vector<int> feelings;

        Collection()
        {
            attraction = 0;
            repulsion = 0;
            time = 0;
            value = 0;
            correlation = 0;
            similarity = 0;
            importance = 0;
            index = -1;
        }

        void set_time()
        {
            unsigned __int64 avg = 0;
            int count = 0;
            for (auto i : events_vector)
            {
                avg += i.time;
                count++;
            }
            time = avg / count;
        }

        // organizes in terms of descending importance
        void organize()
        {
            Event dummy;
            bool done = false;
            while (!done)
            {
                int count = 0;
                for (int i = 0; i < (int)events_vector.size() - 1; i++)
                {
                    if (events_vector.at(i).time < events_vector.at(i + 1).time)
                    {
                        dummy = events_vector.at(i);
                        events_vector.at(i) = events_vector.at(i + 1);
                        events_vector.at(i + 1) = dummy;
                        count++;
                    }
                }
                if (count == 0)
                {
                    done = true;
                }
            }
        }

        // automatically sorts into experiences and actions
        void add_event(Event e)
        {
            events_vector.push_back(e);
            events.push_back(e.index);

            if (e.event_type)
            {
                add_experience(e);
            }
            else
            {
                add_action(e);
            }
        }

        void add_experience(Event e)
        {
            experiences_vector.push_back(e);
        }

        void add_action(Event a)
        {
            actions_vector.push_back(a);
        }

        // have to set the index from outside of the collection
        // or we'll have circular includes. it's the same for all
        // these objects
        void set_index(int id)
        {
            index = id;
        }

        // get this value from Body.evaluate()
        void set_value(double v)
        {
            value = v;
        }

        void set_feeling(Feeling f)
        {
            fln = f;
        }
    };


    // basically a plan of action for the system to take
    // but i called it a sequence instead.
    // too late to change it now, oh well.
    class Sequence
    {
    public:
        int index;
        Collection plan;
        Collection execution;
        std::vector<int> actions_list;
        int current_item;

        // return the action to take next
        // and then advance the plan to the next step
        int advance()
        {
            current_item++;
            return actions_list.at(current_item);
        }

        // returns true if there exists
        // a next action to take
        // advance() will cause an exception if called
        // after this returns false
        bool has_next()
        {
            if (current_item + 1 < (int)actions_list.size())
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // organizes the events in the plan
        // in chronological order
        void organize_plan()
        {
            Event dummy;
            bool done = false;
            while (!done)
            {
                int count = 0;
                for (int i = 0; i < (int)plan.events_vector.size() - 1; i++)
                {
                    if (plan.events_vector.at(i).time < plan.events_vector.at(i + 1).time)
                    {
                        dummy = plan.events_vector.at(i);
                        plan.events_vector.at(i) = plan.events_vector.at(i + 1);
                        plan.events_vector.at(i + 1) = dummy;
                        count++;
                    }
                }
                if (count == 0)
                {
                    done = true;
                }
            }
        }

        // NYI, might not need it
        void organize_execution()
        {

        }

        // create a sequence from a collection
        // supposed to be one of highest value
        void init_sequence(Collection c)
        {
            plan = c;
            for (auto i : c.events_vector)
            {
                if (!i.event_type)
                {
                    actions_list.push_back(i.act.act_type);
                    current_item = 0;
                }
            }
        }

        // adds all the Events in a collection to the
        // sequence's execution vector.
        // its used after the system calls advance()
        // in order to update the execution for what
        // just happened after the system took
        // its action
        void update_execution(Collection c)
        {
            for (auto i : c.events_vector)
            {
                execution.add_event(i);
            }
        }

        void set_index(int id)
        {
            index = id;
        }
    };

    // holds a bunch of collections and evaluates them
    // based on some criteria
    // most of the functions that determine the values
    // of those criteria are in the math module

    // the correlation, similarity, and value
    // of a relation are just the same measures as
    // found in a Collection, but for a Collection
    // that's built by aggregating all the Collections
    // found in the Relation together. So it's kinda
    // just their average values.

    // the presedence is a measure of how causally
    // related two Events are. if an event comes before another event
    // in time, then it will have a positive presedence with respect to that
    // other Event, and if they occured close together in time
    // then the presedence will be high.
    // it's used in a Relation to measure whether or not the
    // events in the relation are in sequential order or not.
    // very similar to the correlation

    // finally, the relativity is a measure of how "related"
    // the collections in this Relation are. it's found by
    // adding the correlation, similarity, and presedence together

    // note: the value of a relation isn't used in calculating the relativity,
    // but it is used for decision making
    class Relation
    {
    public:
        int ind;
        std::vector<int> indices;
        std::vector<Collection> collections;
        double correlation = 0;
        double similarity = 0;
        double value = 0;
        double presedence = 0;
        double relativity = 0;

        void set_index(int id)
        {
            ind = id;
        }

        void add_collection(Collection c)
        {
            indices.push_back(c.index);
            collections.push_back(c);
        }

        void organize()
        {
            Collection dummy;
            bool done = false;
            while (!done)
            {
                if ((int)collections.size() > 1)
                {
                    int count = 0;
                    for (int i = 0; i < (int)collections.size() - 1; i++)
                    {
                        if (collections.at(i).time < collections.at(i + 1).time)
                        {
                            dummy = collections.at(i);
                            collections.at(i) = collections.at(i + 1);
                            collections.at(i + 1) = dummy;
                            count++;
                        }
                    }
                    if (count == 0)
                    {
                        done = true;
                    }
                }
                else
                {
                    std::cout << "Error: cannot organize collection of size less than 2.";
                    std::cout << std::endl;
                    done = true;
                }
            }
        }

        void set_values(std::vector<double> values)
        {
            correlation = values.at(0);
            similarity = values.at(1);
            value = values.at(2);
            presedence = values.at(3);
            relativity = values.at(4);
        }

        void clear_collections()
        {
            collections = {};
        }

        void clear_indices()
        {
            indices = {};
        }
    };






}




