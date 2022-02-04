#pragma once
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>

#include "utilities.h"
#include "data_types.h"
#include "math.h"
#include "Mind.h"

//this class must be initialized by a Lifecycle object in order to be used properly

namespace ocb
{

	class Body
	{
	public:
		/*
			Body()
			{
				for (int i = 0; i < NUMBER_OF_ACTIONS; i++)
				{
					possible_actions[i] = i;
				}
			}

			int possible_actions[NUMBER_OF_ACTIONS];


			char possible_actions_typing[43] = { '\0', '1', '2', '3', '4', '5', '6', '7', '8',
												'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
												'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
												'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
												'y', 'z', '.', ',', '?', ' ', '\n', '9',
												'0' };

			std::string possible_actions_other[10] = { "think", "see", "move up", "move down", "move right", "move left", "zoom in", "zoom out", "evaluate", "evaluate_actions" };
		*/

		// this is the big one, right?
		// we have to give him a way to "think", somehow.
		// this function is the main way we create relations
		// relations are how the program determines
		// which events lead to which other events.
		// 
		// makes a relation kind of at random
		char think(Mind c)
		{
			char w = search_importances(c);
			Collection focus = choose(c.memory);
			int len = 2;

			if (c.questioning <= 1)
			{
				std::cout << "# of collections:";
				std::cout << c.memory.all_collections.size();
				std::cout << std::endl;
				std::cout << "Questioning:";
				std::cout << c.questioning;
				std::cout << std::endl;
				std::cout << "Questioning is too low. No think much...";
				std::cout << std::endl;
				double qtemp = 2;
				len = Math::mistake(2, 10) + (int)(std::log(qtemp) * (std::log(c.memory.all_collections.size())));
			}
			else
			{
				len = Math::mistake(2, 10) + (int)(std::log(c.questioning) * (std::log(c.memory.all_collections.size())));
			}

			//int len = (int)(std::log(c.questioning) * c.memory.all_collections.size());

			std::cout << "Building relation of size: ";
			std::cout << len;
			std::cout << std::endl;

			Relation r;
			r.set_index(c.memory.storage.read_relations_unify());
			c.memory.storage.incr_relations_unify();
			bool done = false;
			while (!done)
			{
				for (auto i : c.memory.all_collections)
				{
					int m = Math::mistake(0, 1);
					if (m == 1)
					{
						r.add_collection(i);
					}
				}
				if ((int)r.collections.size() > len)
				{
					done = true;
				}
				else if (c.memory.all_collections.size() < 5)
				{
					done = true;
				}
				else if (r.collections.size() >= c.memory.all_collections.size())
				{
					done = true;
				}
			}
			r.set_values(Math::calculate_relation_values(r));
			c.memory.add_new_relation(r);
			c.memory.storage.store_relation(r);
			return '\0';
		}

		char think_about(Mind self, Collection c)
		{
			int len = 2;
			if (self.questioning <= 1)
			{
				std::cout << "Questioning is low. Not thinking too hard about this...";
				std::cout << std::endl;
				double qtemp = 2;
				len = Math::mistake(2, 10) + (int)(std::log(qtemp) * (std::log(self.memory.all_collections.size())));
			}
			else
			{
				len = Math::mistake(2,10) + (int)(std::log(self.questioning) * (std::log(self.memory.all_collections.size())));
			}
			//int len = (int)(std::log(self.questioning) * self.memory.all_collections.size());

			std::cout << "Building relation of size: ";
			std::cout << len;
			std::cout << std::endl;

			Relation r;
			r.set_index(self.memory.storage.read_relations_unify());
			self.memory.storage.incr_relations_unify();
			r.add_collection(c);
			bool done = false;
			while (!done)
			{
				for (auto i : self.memory.all_collections)
				{
					int m = Math::mistake(0, 1);
					if (m == 1)
					{
						r.add_collection(i);
					}
				}
				if ((int)r.collections.size() > len)
				{
					done = true;
				}
				else if (self.memory.all_collections.size() < 5)
				{
					done = true;
				}
				else if (r.collections.size() >= self.memory.all_collections.size())
				{
					done = true;
				}
			}
			r.set_values(Math::calculate_relation_values(r));
			self.memory.add_new_relation(r);
			self.memory.storage.store_relation(r);
			return '\0';
		}

		// NYI
		char search_feelings(Mind c)
		{

			return '\0';
		}

		// re-assesses the importance of all the collections in mind
		// then organizes them in order of descending importance
		char search_importances(Mind c)
		{
			for (auto i : c.memory.all_collections)
			{
				i.importance = Math::set_collection_importance(c, i);
			}
			c.memory.organize();
			return '\0';
		}

		//intermediary function
		char forget(Mind c)
		{
			c.memory.all_collections.pop_back();
			return '\0';
		}

		// an intermediary function, not intended
		// as an action the program can take
		char forget_relation(Mind c)
		{
			c.memory.all_relations.pop_back();
			return '\0';
		}

		// used to make a Feeling object
		std::vector<double> get_feelings(Mind self)
		{
			std::vector<double> feelings;
			feelings.push_back(self.happiness);
			feelings.push_back(self.sadness);
			feelings.push_back(self.restlessness);
			feelings.push_back(self.curiosity);
			feelings.push_back(self.questioning);
			feelings.push_back(self.tiredness);
			feelings.push_back(self.tolerance);

			return feelings;
		}

		// this function chooses an action with the intention of sometimes accidentally choosing the wrong one
		// the purpose is to facilitate learning by forcing the program to make mistakes
		// increasing tolerance decreases the randomness
		// dont let tolerance be <= 0 though
		int choose_mistake(std::vector<int> choices)
		{
			double token = 1;
			int choice = 0;
			double tolerance = .7;

			for (int i = 0; i < (int)choices.size(); i++)
			{
				int n = i + 1;
				int v = i + (int)(tolerance * (int)choices.size());
				double rand = (1 / double(n)) + (1 / double(Math::mistake(1, v)));
				if (rand > token)
				{
					token = rand;
					choice = i;
				}
			}

			return choices.at(choice);
		}

		// chooses a memory in a semi-random fashion for the imagine function
		// increasing tolerance decreases the randomness
		// dont let tolerance be <= 0 though
		Collection choose(Memory m)
		{
			double token = 1;
			int choice = 0;
			double tolerance = .4;

			for (int i = 0; i < int(m.all_collections.size()); i++)
			{
				int n = i + 1;
				int v = 2;
				if (m.all_collections.size() > 1)
				{
					v = i + (int)(tolerance * int(m.all_collections.size()));
				}
				//int v = i + (int)(tolerance * int(m.all_collections.size()));
				double rand = (1 / double(n)) + (1 / double(Math::mistake(1, v)));
				if (rand > token && !m.all_collections.at(i).experiences_vector.empty())
				{
					token = rand;
					choice = i;
				}
			}
			return m.all_collections.at(choice);
		}

		// creates a new collection in a semi-random fashion
		// with experiences taken at random and from an 
		// experience with special significance,
		// and then actions which are presedent to those experiences
		char imagine(Mind c)
		{
			if (c.memory.all_collections.size() >= 20)
			{
				c.tiredness += c.tired_tolerance;
			}

			int maximum = Math::mistake(2, 10) * std::log(c.memory.all_collections.size());
			std::cout << "maximum: ";
			std::cout << maximum;
			std::cout << std::endl;
			int length;
			if (maximum > 5)
			{
				length = Math::mistake(2, maximum);
			}
			else if (c.memory.all_collections.size() > 5)
			{
				length = Math::mistake(2, c.memory.all_collections.size());
				std::cout << "Max is too small, using memory size...";
				std::cout << std::endl;
			}
			else
			{
				length = Math::mistake(2, 6);
				std::cout << "Memory size: ";
				std::cout << c.memory.all_collections.size();
				std::cout << std::endl;
				std::cout << "Memory size is too small, using default...";
				std::cout << std::endl;
			}
			int b = Math::mistake(1, length);
			Collection newc;
			Collection inspiration = choose(c.memory);


			std::vector<int> choices = c.memory.storage.read_organized_records();
			bool done = false;
			int counter = 0;
			while (!done)
			{
				int insp_chance = Math::mistake(0, 1);
				if (!inspiration.experiences_vector.empty() && insp_chance == 1)
				{
					newc.add_event(
						inspiration.experiences_vector.at(
							Math::mistake(
								0, inspiration.experiences_vector.size() - 1)));
				}
				int memory_chance = Math::mistake(0, 1);
				if (!c.memory.all_experiences.empty() && memory_chance == 1)
				{
					newc.add_event(
						c.memory.all_experiences.at(
							Math::mistake(
								0, c.memory.all_experiences.size() - 1)));
				}
				int storage_chance = Math::mistake(0, 1);
				if (!choices.empty() && storage_chance == 1)
				{
					Collection c_choice = c.memory.storage.retrieve_collection(
						Math::mistake(
							0, choices.size() - 1));
					Event e_choice = c_choice.events_vector.at(
						Math::mistake(
							0, c_choice.events_vector.size() - 1));
					newc.add_event(e_choice);
				}
				if ((int)newc.experiences_vector.size() >= b)
				{
					done = true;
				}
				else if (counter >= 100)
				{
					std::cout << "Error: caught in infinite imagine loop.";
				}
			}

			// gets some actions which have a high presedence
			// when compared to the experiences we put together
			// and adds them to the collection
			done = false;
			std::vector<Event> dummy;
			while (!done)
			{
				int count = 0;
				for (auto i : newc.experiences_vector)
				{
					dummy = Math::find_presedent_actions(c, i);
					for (int j = 0; j < (int)dummy.size(); j++)
					{
						if ((int)newc.events_vector.size() < length)
						{
							newc.add_event(dummy.at(j));
							count++;
						}
					}
				}
				if (count == 0)
				{
					done = true;
				}
			}
			newc.set_time();
			newc.correlation = Math::get_correlation(newc);
			newc.similarity = Math::get_similarity(newc);
			newc.value = evaluate(c, newc);
			newc.importance = Math::set_collection_importance(c, newc);
			newc.organize();
			c.memory.add_new_thought(newc);

			return '\0';
		}


		// used to assign a "value" to a collection
		double evaluate(Mind self, Collection c)
		{
			// tolerances aren't arranged in proper order, sorry
			double value = 0;
			double tolerance0 = 1.1;
			double tolerance1 = 0.9;
			double tolerance2 = 1;
			double tolerance3 = 1;
			double tolerance4 = 1;
			double tolerance5 = 1;

			// value increases when the collection is similar to the image from the first thought
			// collection similarity ranges from 0 to almost 1
			// and since we want these values to be extreme at the far ends of our range
			// and roughtly equal to zero in the middle
			// we align them to act the way we want with a tangent function
			value = value - (tolerance0 * Math::tangent1(
				Math::get_collection_similarity(
					c, self.first_thought)));

			// value decreases when the collection is similar to the image from the second thought
			value = value + (tolerance1 * Math::tangent1(
				Math::get_collection_similarity(
					c, self.second_thought)));

			value += Math::align1(c.fln.happiness) * self.happy_tolerance;

			value -= Math::align1(c.fln.sadness) * self.sad_tolerance;

			for (auto i : self.memory.all_collections)
			{
				if (!i.experiences_vector.empty())
				{
					// value increases as the collection gets more similar to what's already known in memory
					// again, collection similarity ranges from 0 to 1
					value = value * tolerance2 *
						Math::get_collection_similarity(i, c);

					// it also increases if the collection is wildly dissimilar to what's already known
					// here we align the values to range from a high positive value to a low positive value
					value = value * tolerance3 * Math::tangent1(
						Math::get_collection_similarity(
							c, self.first_thought));

					value = value * Math::tangent1(
						Math::get_collection_similarity(
							c, self.second_thought));
				}
			}

			// value increases as the collection's similarity increases
			value = value * tolerance4 * c.similarity;

			// value increases as the average relativity of the relations
			// that the collection in question appears in increases
			if (self.memory.all_relations.size() > 1)
			{
				value = value * tolerance5 * Math::avg_collection_relativity(self, c);
			}

			return value;
		}

		int evaluate_actions(Mind self)
		{
			int possible_actions[NUMBER_OF_ACTIONS];
			for (int i = 0; i < NUMBER_OF_ACTIONS; i++)
			{
				possible_actions[i] = i;
			}

			std::vector<int> actions_vector = self.memory.actions_index;
			std::vector<int> finished_list;
			double values_index[NUMBER_OF_ACTIONS] = {};
			int actions_index[NUMBER_OF_ACTIONS] = {};
			std::vector<int> actions_index_v;
			for (int i = 0; i < NUMBER_OF_ACTIONS; i++)
			{
				actions_index[i] = i;
				actions_index_v.push_back(i);
			}

			// this looks daunting i guess
			// for each possible action the computer can take
			// we pass every collection that that action appears in
			// to the evaluate() function and see how valuable
			// that collection is
			// the more a given action appears in collections that are valuable,
			// the more it will gain in value itself
			// conversely, if that action appears in collections with negative value
			// the computer will be incentivized to avoid those actions
			for (int i = 0; i < NUMBER_OF_ACTIONS; i++)
			{
				for (int j = 0; j < (int)self.memory.all_collections.size(); j++)
				{
					for (size_t k = 0; k < self.memory.all_collections.at(j).actions_vector.size(); k++)
					{
						Event act = self.memory.all_collections.at(j).actions_vector.at(k);
						if (possible_actions[i] == act.act.act_type)
						{
							values_index[i] = values_index[i] + self.memory.all_collections.at(j).value;
						}
					}
				}
				// also, the more an action appears in general, the less valuable it is
				values_index[i] = values_index[i] - Math::get_action_abundance(self, actions_index[i]);
			}

			// here we're gonna test to determine
			// if there's no good action to take right now
			// if that's so, we'll have the computer value
			// thinking and pulling from long term memory
			// more than taking actions like typing
			double sum = 0;
			double modifier = 0.001;
			for (auto i : values_index)
			{
				sum += i;
			}
			self.value_count += 1;
			self.value_diff = (self.value_diff * (self.value_count - 1) + sum) / self.value_count;
			if (sum < self.value_diff)
			{
				self.questioning += self.quest_tolerance * abs(sum) * modifier;
				self.restlessness -= self.rest_tolerance * sum * modifier;
			}
			else
			{
				self.questioning -= self.quest_tolerance * sum * modifier;
				self.restlessness += self.rest_tolerance * sum * modifier;
			}

			// then we go through and modify the action values
			// by their current feeling modifiers
			for (int i = 0; i < NUMBER_OF_ACTIONS; i++)
			{
				int act_g = Math::get_action_group(i);
				if (act_g == 1)
				{
					// modify the value of typing actions
					// if restlessness is high, will want to type more
					values_index[i] = values_index[i] * Math::cotangent1(self.rest_tolerance * self.restlessness);
				}
				else if (act_g == 2)
				{
					// if curiosity is high, will want to see more
					values_index[i] = values_index[i] * Math::cotangent1(self.curious_tolerance * self.curiosity);
				}
				else if (act_g == 3)
				{
					// if questioning is high, will want to think more
					values_index[i] = values_index[i] * Math::cotangent1(self.quest_tolerance * self.questioning);
				}
				// sleeping goes here in the future
			}

			// here we're just doing funky programming stuff
			// in order to organize the list of all possible actions
			// in descending order of how valuable each action is
			bool done = false;
			int best_action;
			while (!done)
			{
				int count = 0;
				for (int i = 0; i < NUMBER_OF_ACTIONS - 1; i++)
				{
					if (values_index[i] < values_index[i + 1])
					{
						int dummy = actions_index[i + 1];
						actions_index[i + 1] = actions_index[i];
						actions_index[i] = dummy;
						double dummy2 = values_index[i + 1];
						values_index[i + 1] = values_index[i];
						values_index[i] = dummy2;
						count++;
					}
				}
				if (count < 1)
				{
					done = true;
				}

				//for (int i = 0; i < NUMBER_OF_ACTIONS; i++)
				//{
				//	finished_list.push_back(actions_index[i]);
				//}
				// at this point, we're giving the computer the chance
				// to make a mistake (albeit a low chance)
				// this helps to develop new ideas down the road
				// by forcing it to experiment sometimes
				//best_action = choose_mistake(finished_list);
			}
			best_action = actions_index[0];
			self.happiness += values_index[0] * self.happy_tolerance;
			self.sadness -= values_index[0] * self.sad_tolerance;

			return best_action;
		}


		char see(Vision v)
		{
			cv::Mat vis = v.take_screenshot();
			cv::imwrite("sight1.jpg", vis);
			return '\0';
		}

		char see_webcam(Vision v)
		{
			cv::Mat vis = v.capture_webcam();
			cv::imwrite("sight2.jpg", vis);
			return '\0';
		}

		char move_up(Vision v)
		{
			v.move_up();
			return '\0';
		}

		char move_down(Vision v)
		{
			v.move_down();
			return '\0';
		}

		char move_right(Vision v)
		{
			v.move_right();
			return '\0';
		}

		char move_left(Vision v)
		{
			v.move_left();
			return '\0';
		}

		char zoom_out(Vision v)
		{
			v.zoom_out();
			return '\0';
		}

		char zoom_in(Vision v)
		{
			v.zoom_in();
			return '\0';
		}

		char webcam_move_up(Vision v)
		{
			v.webcam_move_up();
			return '\0';
		}

		char webcam_move_down(Vision v)
		{
			v.webcam_move_down();
			return '\0';
		}

		char webcam_move_right(Vision v)
		{
			v.webcam_move_right();
			return '\0';
		}

		char webcam_move_left(Vision v)
		{
			v.webcam_move_left();
			return '\0';
		}

		char webcam_zoom_out(Vision v)
		{
			v.webcam_zoom_out();
			return '\0';
		}

		char webcam_zoom_in(Vision v)
		{
			v.webcam_zoom_in();
			return '\0';
		}

		char type_nothing()
		{
			return '\0';
		}

		char type_a()
		{
			return 'a';
		}

		char type_b()
		{
			return 'b';
		}

		char type_c()
		{
			return 'c';
		}

		char type_d()
		{
			return 'd';
		}

		char type_e()
		{
			return 'e';
		}

		char type_f()
		{
			return 'f';
		}

		char type_g()
		{
			return 'g';
		}

		char type_h()
		{
			return 'h';
		}

		char type_i()
		{
			return 'i';
		}

		char type_j()
		{
			return 'j';
		}

		char type_k()
		{
			return 'k';
		}

		char type_l()
		{
			return 'l';
		}

		char type_m()
		{
			return 'm';
		}

		char type_n()
		{
			return 'n';
		}

		char type_o()
		{
			return 'o';
		}

		char type_p()
		{
			return 'p';
		}

		char type_q()
		{
			return 'q';
		}

		char type_r()
		{
			return 'r';
		}

		char type_s()
		{
			return 's';
		}

		char type_t()
		{
			return 't';
		}

		char type_u()
		{
			return 'u';
		}

		char type_v()
		{
			return 'v';
		}

		char type_w()
		{
			return 'w';
		}

		char type_x()
		{
			return 'x';
		}

		char type_y()
		{
			return 'y';
		}

		char type_z()
		{
			return 'z';
		}

		char type_1()
		{
			return '1';
		}

		char type_2()
		{
			return '2';
		}

		char type_3()
		{
			return '3';
		}

		char type_4()
		{
			return '4';
		}

		char type_5()
		{
			return '5';
		}

		char type_6()
		{
			return '6';
		}

		char type_7()
		{
			return '7';
		}

		char type_8()
		{
			return '8';
		}

		char type_9()
		{
			return '9';
		}

		char type_0()
		{
			return '0';
		}

		char type_period()
		{
			return '.';
		}

		char type_comma()
		{
			return ',';
		}

		char type_question()
		{
			return '?';
		}

		char type_space()
		{
			return ' ';
		}

		char type_newline()
		{
			return '\n';
		}

		char type_letter(int id)
		{
			if (id == 0)
			{
				return type_nothing();
			}

			if (id == 1)
			{
				return type_1();
			}

			else if (id == 2)
			{
				return type_2();
			}

			else if (id == 3)
			{
				return type_3();
			}

			else if (id == 4)
			{
				return type_4();
			}

			else if (id == 5)
			{
				return type_5();
			}

			else if (id == 6)
			{
				return type_6();
			}

			else if (id == 7)
			{
				return type_7();
			}

			else if (id == 8)
			{
				return type_8();
			}

			else if (id == 9)
			{
				return type_9();
			}

			else if (id == 0)
			{
				return type_0();
			}

			else if (id == 11)
			{
				return type_a();
			}

			else if (id == 12)
			{
				return type_b();
			}

			else if (id == 13)
			{
				return type_c();
			}

			else if (id == 14)
			{
				return type_d();
			}

			else if (id == 15)
			{
				return type_e();
			}

			else if (id == 16)
			{
				return type_f();
			}

			else if (id == 17)
			{
				return type_g();
			}

			else if (id == 18)
			{
				return type_h();
			}

			else if (id == 19)
			{
				return type_i();
			}

			else if (id == 20)
			{
				return type_j();
			}

			else if (id == 21)
			{
				return type_k();
			}

			else if (id == 22)
			{
				return type_l();
			}

			else if (id == 23)
			{
				return type_m();
			}

			else if (id == 24)
			{
				return type_n();
			}

			else if (id == 25)
			{
				return type_o();
			}

			else if (id == 26)
			{
				return type_p();
			}

			else if (id == 27)
			{
				return type_q();
			}

			else if (id == 28)
			{
				return type_r();
			}

			else if (id == 29)
			{
				return type_s();
			}

			else if (id == 30)
			{
				return type_t();
			}

			else if (id == 31)
			{
				return type_u();
			}

			else if (id == 32)
			{
				return type_v();
			}

			else if (id == 33)
			{
				return type_w();
			}

			else if (id == 34)
			{
				return type_x();
			}

			else if (id == 35)
			{
				return type_y();
			}

			else if (id == 36)
			{
				return type_z();
			}

			else if (id == 37)
			{
				return type_period();
			}

			else if (id == 38)
			{
				return type_comma();
			}

			else if (id == 39)
			{
				return type_question();
			}

			else if (id == 40)
			{
				return type_space();
			}

			else if (id == 41)
			{
				return type_newline();
			}
			else
			{
				return '\0';
			}
		}

		char take_sight_action(Vision v, int id)
		{
			if (id == 42)
			{
				return move_up(v);
			}

			else if (id == 43)
			{
				return move_down(v);
			}

			else if (id == 44)
			{
				return move_right(v);
			}

			else if (id == 45)
			{
				return move_left(v);
			}

			else if (id == 46)
			{
				return zoom_in(v);
			}

			else if (id == 47)
			{
				return zoom_out(v);
			}

			else if (id == 48)
			{
				return see(v);
			}

			else if (id == 49)
			{
				return webcam_move_up(v);
			}

			else if (id == 50)
			{
				return webcam_move_down(v);
			}

			else if (id == 51)
			{
				return webcam_move_right(v);
			}

			else if (id == 52)
			{
				return webcam_move_left(v);
			}

			else if (id == 53)
			{
				return webcam_zoom_in(v);
			}

			else if (id == 54)
			{
				return webcam_zoom_out(v);
			}

			else if (id == 55)
			{
				return see_webcam(v);
			}

			else
			{
				return '\0';
			}
		}

		char take_other_action(Mind c, int id)
		{
			if (id == 56)
			{
				return think(c);
			}

			else if (id == 57)
			{
				return imagine(c);
			}

			else if (id == 58)
			{
				return search_importances(c);
			}

			else if (id == 59)
			{
				return search_feelings(c);
			}

			else
			{
				return '\0';
			}
		}
	};

}

