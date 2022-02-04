#pragma once

#include <Windows.h>
#include <iostream>

#include <thread>
#include "utilities.h"
#include "math.h"
#include "Body.h"

namespace ocb
{


    class Lifecycle
    {


    public:

        bool existing;
        Mind mind;
        Body body;

        Event new_event;
        Event next_event;

        int active_thoughts;
        int active_imagines;
        bool sleepy;
        int sleep_count;




        char current_act;

        Lifecycle(bool turn_on)
        {
            existing = true;
            active_thoughts = 0;
            active_imagines = 0;
            sleepy = false;
            sleep_count = 0;
            mind = new Mind(turn_on);
            current_act = '\0';
        }

        Lifecycle()
        {
            //std::cout << "Hello World!\n";

            //arrive();
            existing = true;
            active_thoughts = 0;
            active_imagines = 0;
            sleepy = false;
            sleep_count = 0;
            current_act = '\0';
        }



        void Lifecycle_copy(const Lifecycle& orig)
        {
            existing = orig.existing;
            active_thoughts = orig.active_thoughts;
            active_imagines = orig.active_imagines;
            sleepy = orig.sleepy;
            mind = orig.mind;
            body = orig.body;
        }

        void exist()
        {
                if (!sleeping())
                {
                    if (!tired())
                    {
                        if (has_plan())
                        {


                            //auto live_bind = std::bind(&Lifecycle::live, this);
                            //auto imagine_bind = std::bind(&Lifecycle::imagining, this);
                            //auto thinking_bind = std::bind(&Lifecycle::thinking);
                            std::thread live_thread(&Lifecycle::live, this);
                            std::thread imagine_thread(&Lifecycle::imagining, this);
                            std::thread thinking_thread(&Lifecycle::thinking, this);
                            
                            live_thread.join();
                            imagine_thread.join();
                            thinking_thread.join();
                            
                        }
                        else
                        {

                            /*auto live_bind = std::bind(&Lifecycle::live_no_plan);
                            auto imagine_bind = std::bind(&Lifecycle::imagining);
                            auto thinking_bind = std::bind(&Lifecycle::thinking);*/
                            std::thread live_thread(&Lifecycle::live_no_plan, this);
                            std::thread imagine_thread(&Lifecycle::imagining, this);
                            std::thread thinking_thread(&Lifecycle::thinking, this);


                            live_thread.join();
                            imagine_thread.join();
                            thinking_thread.join();
                        }
                    }
                    else
                    {
                        fall_asleep();
                    }
                }
                else
                {
                    if (!tired())
                    {
                        wake_up();
                    }
                    else
                    {
                        sleep();
                    }
                }
        }

        void live()
        {
            std::cout << "living with a plan";
            std::cout << std::endl;

            // see the screen and process the image
            experience_something();

            // pick the best action to take
            if (mind.memory.plans.empty())
            {
                mind.memory.create_plan(0);
                Math::set_sequence_execution(mind, mind.memory.plans.back());
            }

            do_something_planned();


            // think and imagine


            // put all that in memory
            Collection new_thought;

            new_thought.add_event(new_event);
            new_thought.add_event(next_event);

            new_thought.set_time();
            new_thought.organize();
            new_thought.correlation = Math::get_correlation(new_thought);
            new_thought.similarity = Math::get_similarity(new_thought);
            new_thought.set_value(body.evaluate(mind, new_thought));
            new_thought.importance = Math::set_collection_importance(mind, new_thought);
            new_thought.set_index(mind.memory.storage.read_unify());
            mind.memory.storage.incr_unify();

            mind.memory.add_event(new_event);
            mind.memory.add_event(next_event);

            mind.memory.add_memory(new_thought);
            mind.memory.current_plan.update_execution(new_thought);
            mind.memory.storage.store_collection(new_thought);

            create_plan();
            mind.memory.current_plan.update_execution(new_thought);

            // take action
            std::cout << "Next action: ";
            std::cout << next_event.act.act_type;
            std::cout << '\n';
            int act_group = Math::get_action_group(next_event.act.act_type);
            char response;
            if (act_group == 1)
            {
                std::cout << "typing action";
                std::cout << std::endl;
                mind.restlessness -= mind.rest_tolerance;
                mind.curiosity += mind.curious_tolerance;
                mind.tiredness += mind.tired_tolerance;
                response = body.type_letter(next_event.act.act_type);
            }
            else if (act_group == 2)
            {
                std::cout << "seeing action";
                std::cout << std::endl;
                mind.curiosity -= mind.curious_tolerance;
                mind.questioning += mind.quest_tolerance;
                mind.tiredness += mind.tired_tolerance;
                response = body.take_sight_action(mind.eye1, next_event.act.act_type);
            }
            else if (act_group == 3)
            {
                std::cout << "thinking action";
                std::cout << std::endl;
                mind.questioning -= mind.quest_tolerance;
                mind.restlessness += mind.rest_tolerance;
                mind.tiredness += mind.tired_tolerance;
                response = body.take_other_action(mind, next_event.act.act_type);
            }
            else
            {
                std::cout << "Error: unknown action type\n";
                response = '\0';
            }
            
            current_act = response;
            //return response;
        }

        std::string sleep()
        {
            mind.tiredness -= 1;
            int last = mind.memory.all_collections.size();
            int access = last - 1 - sleep_count;
            sleep_count++;
            if (access >= 0)
            {
                mind.memory.all_collections.at(access).value = body.evaluate(mind, mind.memory.all_collections.at(access));
                mind.memory.storage.store_collection(mind.memory.all_collections.at(access));
            }
            int access2 = mind.memory.all_relations.size() - 1 - sleep_count;
            if (access2 >= 0)
            {
                mind.memory.storage.store_relation(mind.memory.all_relations.at(access2));
            }
            return "\nsleeping...\n";
        }

        bool tired()
        {
            std::cout << "Tiredness: ";
            std::cout << mind.tiredness;
            std::cout << std::endl;
            //reset this from 15 to 50 or whatever is needed later when done debugging
            if (mind.tiredness >= 50)
            {
                std::cout << "definitely tired";
                std::cout << std::endl;
                return true;
            }
            else
            {
                std::cout << "not tired yet";
                std::cout << std::endl;
                return false;
            }
        }

        bool awake()
        {
            std::cout << "Tiredness: ";
            std::cout << mind.tiredness;
            std::cout << std::endl;
            if (mind.tiredness <= 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        void fall_asleep()
        {
            sleepy = true;
            sleep_count = 0;
            bool done = false;
            // wait for imagine and thought threads to end
            std::cout << "Waiting for thoughts to end...";
            std::cout << std::endl;
            while (!done)
            {
                if (active_thoughts == 0 && active_imagines == 0)
                {
                    done = true;
                }
            }
            // do initial sleeping things
            body.search_importances(mind);
            body.search_feelings(mind);

            // output the current feelings cause i want to know
            std::cout << "Happiness: ";
            std::cout << mind.happiness;
            std::cout << std::endl;
            std::cout << "Sadness: ";
            std::cout << mind.sadness;
            std::cout << std::endl;
            std::cout << "Restlessness: ";
            std::cout << mind.restlessness;
            std::cout << std::endl;
            std::cout << "Curiosity: ";
            std::cout << mind.curiosity;
            std::cout << std::endl;
            std::cout << "Questioning: ";
            std::cout << mind.questioning;
            std::cout << std::endl;
            std::cout << "Tiredness: ";
            std::cout << mind.tiredness;
            std::cout << std::endl;
            std::cout << "Tolerance: ";
            std::cout << mind.tolerance;
            std::cout << std::endl;
            std::cout << "Falling asleep...";
            std::cout << std::endl;

            synchronize();
        }

        bool sleeping()
        {
            return sleepy;
        }

        void wake_up()
        {
            std::cout << "Waking up...";
            std::cout << std::endl;
            mind.memory.storage.organize_storage();
            std::cout << "Organizing storage...";
            std::cout << std::endl;
            std::cout << "Current # of collections: ";
            std::cout << mind.memory.all_collections.size();
            std::cout << std::endl;
            mind.memory.clear_memory();
            std::cout << "Clearing memory...";
            std::cout << std::endl;
            std::cout << "Current # of collections: ";
            std::cout << mind.memory.all_collections.size();
            std::cout << std::endl;
            mind.memory.init_memory();
            std::cout << "Initializing memory...";
            std::cout << std::endl;
            std::cout << "Current # of collections: ";
            std::cout << mind.memory.all_collections.size();
            std::cout << std::endl;
            sleep_count = 0;
            sleepy = false;
        }

        bool has_plan()
        {
            std::cout << "Checking for plans";
            std::cout << std::endl;
            if (mind.memory.plans.empty())
            {
                return false;
            }

            if (mind.memory.plans.size() > MAX_PLANS)
            {
                std::cout << "pruning excess plans...";
                std::cout << std::endl;
                for (int i = 0; i < (int)mind.memory.plans.size() - MAX_PLANS; i++)
                {
                    mind.memory.plans.erase(mind.memory.plans.begin());
                }
                mind.questioning -= mind.quest_tolerance;
                mind.tiredness += mind.tired_tolerance;
            }

            std::cout << "iterating over plans";
            std::cout << std::endl;
            for (int i = 0; i < (int)mind.memory.plans.size(); i++)
            {/*
                std::cout << "test5-20-2 checking if plan has a next action";
                std::cout << std::endl;*/
                int choice = mind.memory.plans.size() - 1 - i;
                if (mind.memory.plans.at(choice).has_next())
                {/*
                    std::cout << "test5-21 checking if the plan is still valid";
                    std::cout << std::endl;*/
                    if (Math::check_sequence(mind, mind.memory.plans.at(choice)))
                    {
                        std::cout << "using this plan";
                        std::cout << std::endl;
                        mind.memory.current_plan = mind.memory.plans.at(choice);
                        return true;
                    }
                }
            }

            std::cout << "no plans worked";
            std::cout << std::endl;
            return false;
        }

        void live_no_plan()
        {
            std::cout << "living without a plan";
            std::cout << std::endl;

            experience_something();

            do_something_unplanned();


            // put all that in memory
            Collection new_thought;


            new_thought.add_event(new_event);
            new_thought.add_event(next_event);

            new_thought.set_time();
            new_thought.organize();
            new_thought.correlation = Math::get_correlation(new_thought);
            new_thought.similarity = Math::get_similarity(new_thought);
            new_thought.set_value(body.evaluate(mind, new_thought));
            new_thought.importance = Math::set_collection_importance(mind, new_thought);

            new_thought.set_index(mind.memory.storage.read_unify());
            mind.memory.storage.incr_unify();
            std::cout << "created new thought";
            std::cout << std::endl;

            mind.memory.add_event(new_event);
            mind.memory.add_event(next_event);

            mind.memory.add_collection(new_thought);
            mind.memory.storage.store_collection(new_thought);

            create_plan();
            mind.memory.current_plan.update_execution(new_thought);

            // take action
            std::cout << "Next action: ";
            std::cout << next_event.act.act_type;
            std::cout << '\n';

            int act_group = Math::get_action_group(
                next_event.act.act_type);

            char return_action;

            if (act_group == 1)
            {
                std::cout << "typing action: ";
                std::cout << std::endl;
                mind.restlessness -= mind.rest_tolerance;
                mind.curiosity += mind.curious_tolerance;
                mind.tiredness += mind.tired_tolerance;
                return_action = body.type_letter(next_event.act.act_type);
            }
            else if (act_group == 2)
            {
                std::cout << "seeing action: ";
                std::cout << std::endl;
                mind.curiosity -= mind.curious_tolerance;
                mind.questioning += mind.quest_tolerance;
                mind.tiredness += mind.tired_tolerance;
                return_action = body.take_sight_action(mind.eye1, next_event.act.act_type);
            }
            else if (act_group == 3)
            {
                std::cout << "thinking action: ";
                std::cout << std::endl;
                mind.questioning -= mind.quest_tolerance;
                mind.restlessness += mind.rest_tolerance;
                mind.tiredness += mind.tired_tolerance;
                return_action = body.take_other_action(mind, next_event.act.act_type);
            }
            else
            {
                std::cout << "Error: unknown action type\n";
                return_action = '\0';
            }

            current_act = return_action;
        }

        void create_plan()
        {
            bool done = false;
            int count = 0;
            int count2 = 0;
            std::vector<int> indices = mind.memory.storage.read_organized_records();
            std::cout << "creating a plan in a while loop...";
            std::cout << std::endl;
            while (!done)
            {
                int choice = mind.memory.all_collections.size() - 1 - count;
                if (choice >= 0)
                {/*
                    std::cout << "test5-8 creating a plan";
                    std::cout << std::endl;*/
                    mind.memory.create_plan(choice);/*
                    std::cout << "test5-10 updating its execution";
                    std::cout << std::endl;*/
                    Math::set_sequence_execution(mind, mind.memory.plans.back());/*
                    std::cout << "test5-11 checking the plan to see if its valid";
                    std::cout << std::endl;*/
                    done = Math::check_sequence(mind, mind.memory.plans.back());/*
                    std::cout << "test5-12 updating feelings";
                    std::cout << std::endl;*/
                    count++;
                    if (done)
                    {
                        mind.memory.current_plan = mind.memory.plans.back();
                    }
                    mind.restlessness += mind.rest_tolerance / mind.memory.all_collections.size();
                    mind.tolerance += mind.tol_tolerance / mind.memory.all_collections.size();
                    mind.questioning -= mind.quest_tolerance / mind.memory.all_collections.size();
                    mind.curiosity += mind.curious_tolerance / mind.memory.all_collections.size();
                    mind.tiredness += mind.tired_tolerance / mind.memory.all_collections.size();
                }
                else if (count2 < (int)indices.size()
                    && (mind.questioning > mind.restlessness
                        || mind.questioning > mind.curiosity))
                {
                    std::cout << "adding a collection to memory from storage";
                    std::cout << std::endl;
                    int choice = indices.at(
                        indices.size() - 1 - count2);/*
                    std::cout << "test5-13-2 retrieving collection at index : ";
                    std::cout << choice;*/
                    std::cout << std::endl;
                    mind.memory.add_collection(
                        mind.memory.storage.retrieve_collection(
                            choice));/*
                    std::cout << "test5-14 updating feelings";
                    std::cout << std::endl;*/
                    count2++;
                    mind.restlessness += mind.rest_tolerance / indices.size();
                    mind.tolerance -= mind.tol_tolerance / indices.size();
                    mind.questioning -= mind.quest_tolerance / indices.size();
                    mind.curiosity += mind.curious_tolerance / indices.size();
                    mind.tiredness += mind.tired_tolerance / indices.size();
                }
                else if (count2 > 1000)
                {
                    std::cout << "Error: caught in infinite planning loop. Exiting...\n";
                    done = true;
                }
                else
                {
                    done = true;
                    std::cout << "Failed to create a valid plan. Exiting loop...";
                    std::cout << std::endl;
                }
            }
        }

        void synchronize()
        {
            for (auto i : mind.memory.all_collections)
            {
                if (i.index == -1)
                {
                    i.set_index(mind.memory.storage.read_unify());
                    mind.memory.storage.incr_unify();
                }
            }
        }

        void arrive()
        {
            std::cout << "\nstarting arrival\n";
            mind.first_thought.set_time();
            mind.first_thought.correlation = Math::get_correlation(mind.first_thought);
            mind.first_thought.similarity = Math::get_similarity(mind.first_thought);
            mind.first_thought.value = body.evaluate(mind, mind.first_thought);
            mind.first_thought.importance = Math::set_collection_importance(mind, mind.first_thought);
            mind.first_thought.set_index(mind.memory.storage.read_unify());
            mind.memory.storage.incr_unify();

            mind.second_thought.set_time();
            mind.second_thought.correlation = Math::get_correlation(mind.second_thought);
            mind.second_thought.similarity = Math::get_similarity(mind.second_thought);
            mind.second_thought.value = body.evaluate(mind, mind.second_thought);
            mind.second_thought.importance = Math::set_collection_importance(mind, mind.second_thought);
            mind.second_thought.set_index(mind.memory.storage.read_unify());
            mind.memory.storage.incr_unify();

            mind.third_thought.set_time();
            mind.third_thought.correlation = Math::get_correlation(mind.third_thought);
            mind.third_thought.similarity = Math::get_similarity(mind.third_thought);
            mind.third_thought.value = body.evaluate(mind, mind.third_thought);
            mind.third_thought.importance = Math::set_collection_importance(mind, mind.third_thought);
            mind.third_thought.set_index(mind.memory.storage.read_unify());
            mind.memory.storage.incr_unify();

            mind.fourth_thought.set_time();
            mind.fourth_thought.correlation = Math::get_correlation(mind.fourth_thought);
            mind.fourth_thought.similarity = Math::get_similarity(mind.fourth_thought);
            mind.fourth_thought.value = body.evaluate(mind, mind.fourth_thought);
            mind.fourth_thought.importance = Math::set_collection_importance(mind, mind.fourth_thought);
            mind.fourth_thought.set_index(mind.memory.storage.read_unify());
            mind.memory.storage.incr_unify();

            mind.memory.add_collection(mind.first_thought);
            mind.memory.add_collection(mind.second_thought);
            mind.memory.add_collection(mind.third_thought);
            mind.memory.add_collection(mind.fourth_thought);

            mind.memory.storage.store_collection(mind.first_thought);
            mind.memory.storage.store_collection(mind.second_thought);
            mind.memory.storage.store_collection(mind.third_thought);
            mind.memory.storage.store_collection(mind.fourth_thought);

            mind.memory.organize();
            mind.memory.storage.organize_storage();
            std::cout << "\nending arrival\n";

        }

        void experience_something()
        {
            Feeling feel;
            feel.set_feelings(body.get_feelings(mind));
            char r = body.see(mind.eye1);
            Experience new_exp;
            new_event.set_feeling(feel);
            new_exp.set_image(Image::process_image(Image::grayscale()));
            new_event.time = mind.t.get_current_time();
            new_event.set_index(mind.memory.storage.read_events_unify());
            mind.memory.storage.incr_events_unify();
            std::cout << "new seeing event at index: ";
            std::cout << new_event.index;
            std::cout << std::endl;
            new_event.set_experience(new_exp);
        }

        void do_something_planned()
        {
            Feeling feel;
            feel.set_feelings(body.get_feelings(mind));
            Action next_action;
            next_event.set_feeling(feel);
            next_action.set_act_type(mind.memory.current_plan.advance());
            next_event.time = mind.t.get_current_time();
            next_event.set_index(mind.memory.storage.read_events_unify());
            mind.memory.storage.incr_events_unify();
            std::cout << "successfully picked an action of type: ";
            std::cout << next_action.act_type;
            std::cout << std::endl;
            next_event.set_action(next_action);
        }

        void do_something_unplanned()
        {
            Feeling feel;
            feel.set_feelings(body.get_feelings(mind));
            Action next_action;
            next_event.set_feeling(feel);
            //int a = mind.memory.plans.back().advance();
            next_action.set_act_type(body.evaluate_actions(mind));
            next_event.time = mind.t.get_current_time();
            next_event.set_index(mind.memory.storage.read_events_unify());
            mind.memory.storage.incr_events_unify();
            std::cout << "new action event";
            std::cout << std::endl;
            next_event.set_action(next_action);
        }

        void thinking()
        {
            std::string rtn_string;
            if (!sleepy && active_thoughts == 0)
            {
                active_thoughts++;
                rtn_string.append("Thinking...");
                char z = body.think(mind);
                active_thoughts--;
            }
            else
            {
                rtn_string.append("Already thinking...");
            }
            //return '\0';
        }

        void imagining()
        {
            std::string rtn_string;
            if (!sleepy && active_imagines == 0)
            {
                active_imagines++;
                rtn_string.append( "Imagining...\n");
                char q = body.imagine(mind);
                active_imagines--;
            }
            else
            {
                rtn_string.append("Already imagining...\n");
            }
            //return '\0';
        }



    };




}
