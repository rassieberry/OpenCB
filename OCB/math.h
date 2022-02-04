#pragma once
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <random>

#include "utilities.h"
#include "data_types.h"
#include "Mind.h"

namespace ocb
{

	class Math
	{
	public:

		// often used function that takes an input 
		// ranging from 0 to +inf and 
		// aligns it into a domain that
		// goes from 0 to 1
		// output increases as input increases
		static double align1(double x)
		{
			double r = -1 / (x + 1) + 1;
			return r;
		}

		// variation of the align1 function
		// which takes an input ranging
		// from 0 to +inf and
		// aligns it to a domain that goes 
		// from 0 to 1. the difference here is that
		// higher values of inputs yield lower 
		// values of outputs
		static double align1_inv(double x)
		{
			double r = 1 / (x + 1);
			return r;
		}

		// cotangent function that aligns an input
		// ranging from -inf to +inf
		// into a result domain that ranges from 0 to 1
		static double cotangent1(double x)
		{
			double r = 1 / tan(x / (2 * MATH_PI)) - 1;
			return r;
		}

		// tangent function that aligns an input
		// ranging from 0 to 1
		// into a result domain that ranges from 
		// a high positive number (not sure which)
		// to just above 1
		static double tangent1(double x)
		{
			//std::tan(
			//	(Math::get_collection_similarity(c, self.first_thought) - 1) / MATH_PI))
			double r = 1 + std::tan((x - 0.9) / MATH_PI);
			return r;
		}

		//randomizer function
		static int mistake(int min, int max)
		{
			//std::random_device rd;
			unsigned long long int t = Time::get_time();
			std::mt19937 rng(t);
			std::uniform_int_distribution<int> uni(min, max);
			int r = (int)uni(rng);
			return r;
		}

		// here we check how close together in time 
		// these two collections happened
		// ranges from 0 to 1
		static double get_collection_correlation(Collection c1, Collection c2)
		{
			Collection test;
			for (auto i : c1.events_vector)
			{
				test.add_event(i);
			}
			for (auto j : c2.events_vector)
			{
				test.add_event(j);
			}
			return get_correlation(test);
		}

		// here we check how close together in time
		// the events in a collection happened
		// ranges from 0 to 1
		static double get_correlation(Collection c)
		{
			double average = double(c.time);

			double s = 0;

			for (auto i : c.events_vector)
			{
				s = s + abs(average - i.time);
			}

			double correlation = align1_inv(s);
			return correlation;
		}

		// here we figure out how similar 
		// the images in this collection are to each other
		// ranges from 0 to 1
		static double get_similarity(Collection c)
		{
			double sim = 0;
			int count = 0;/*
			std::cout << "getting similarity of collection size: ";
			std::cout << c.experiences_vector.size();
			std::cout << std::endl;*/
			if (c.experiences_vector.size() > 1)
			{
				for (int i = 0; i < (int)c.experiences_vector.size(); i++)
				{
					for (int j = 0; j < IMG_DIVISIONS; j++)
					{
						if (!c.experiences_vector.at(i).exp.this_image.empty())
						{/*
							std::cout << "Valid image found";
							std::cout << std::endl;*/
							for (int k = 0; k < c.experiences_vector.at(i).exp.this_image.at(j).cols; k++)
							{
								int temp = 0;
								for (int p = 1; p < c.experiences_vector.at(i).exp.this_image.at(j).rows; p++)
								{

									temp = abs(c.experiences_vector.at(i).exp.this_image.at(j).at<char>(cv::Point(k, p))
										- c.experiences_vector.at(i).exp.this_image.at(j).at<char>(cv::Point(k, p - 1))
									);
									sim = sim + temp;
									count++;
								}
							}
						}
						else
						{
							std::cout << "Error: tried to get similarity of an experience with no image at index: ";
							std::cout << c.experiences_vector.at(i).index;
							std::cout << std::endl;
						}
					}
				}
			}
			if (count > 0)
			{
				sim = sim / count;
			}

			sim = align1_inv(sim);

			return sim;
		}

		static double get_image_similarity(cv::Mat image1, cv::Mat image2)
		{

		}

		static double get_image_sim_values(cv::Mat image1, cv::Mat image2)
		{
			int temp = 0;
			double value_sim = 0;
			int count = 0;
			for (int i = 0; i < image1.cols; i++)
			{
				for (int j = 1; j < image1.rows; j++)
				{
					temp = abs(image1.at<char>(cv::Point(i, j)) - image2.at<char>(cv::Point(i, j - 1)));
					value_sim += temp;
					count++;
				}
			}
			if (count > 0)
			{
				value_sim = value_sim / count;
			}
			return value_sim;
		}

		static double get_image_sim_contours(cv::Mat image1, cv::Mat image2)
		{
			double contour_sim = 0;
			int temp = 0;
			int count = 0;
			cv::Mat canny_out;
			cv::Scalar thresh1 = cv::mean(image1);
			cv::Scalar thresh2 = thresh1[0] + ((MAX_COLOR - thresh1[0]) / 2);
			cv::Canny(image1, canny_out, thresh1[0], thresh2[0]);
			std::vector<std::vector<cv::Point>> contours;
			std::vector<cv::Vec4i> hierarchy;
			cv::findContours(canny_out, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
		}


		// with this function we put two collections together and then
		// try to figure out how similar the images from both collections
		// are to each other
		// ranges from 0 to 1
		static double get_collection_similarity(Collection c1, Collection c2)
		{/*
			double sim = 0;
			int count = 0;*/

			Collection test;/*
			std::cout << "getting collections similarity: ";
			std::cout << std::endl;*/
			for (auto i : c1.events_vector)
			{
				test.add_event(i);/*
				std::cout << "adding event at index: ";
				std::cout << i.index;
				std::cout << std::endl;*/
			}
			for (auto j : c2.events_vector)
			{
				test.add_event(j);/*
				std::cout << "adding event at index: ";
				std::cout << j.index;
				std::cout << std::endl;*/
			}
			return get_similarity(test);
			/*
			std::vector<Event> exp_vect;
			if (!c1.experiences_vector.empty() && !c2.experiences_vector.empty())
			{
				for (auto i : c1.experiences_vector)
				{
					exp_vect.push_back(i);
				}

				for (auto i : c2.experiences_vector)
				{
					exp_vect.push_back(i);
				}

				for (int i = 0;  i < (int)exp_vect.size(); i++)
				{
					for (int j = 0; j < IMG_DIVISIONS; j++)
					{
						for (int k = 0; k < exp_vect.at(i).exp.this_image.at(j).cols - 1; k++)
						{
							for (int p = k; p < exp_vect.at(i).exp.this_image.at(j).rows; p++)
							{
								if (p + 1 < exp_vect.at(i).exp.this_image.at(j).rows)
								{
									sim = sim + abs(exp_vect.at(i).exp.this_image.at(j).at<char>(cv::Point(k, p)) - exp_vect.at(i).exp.this_image.at(j).at<char>(cv::Point(k, p + 1)));
									count++;
								}
							}
						}
					}
				}
			}
			if (count > 0)
			{
				sim = sim / count;
				sim = align1(sim);
			}
			return sim;
			*/
		}

		static double set_collection_importance(Mind self, Collection c)
		{
			double sum = 0.1;
			double importance = 0;
			for (auto i : self.memory.all_relations)
			{
				bool in_relation = false;
				for (auto j : i.indices)
				{
					if (j == c.index)
					{
						in_relation = true;
					}
				}
				if (in_relation)
				{
					sum += i.relativity;
				}
			}
			importance = align1(sum);
			return importance;
		}
		// this function returns the relative abundance of a 
		// specific action in the computer's memory
		// basically, how often that action has been taken
		// it'll be aligned on a scale ranging from 0 to almost 1
		static double get_action_abundance(Mind self, int act_type)
		{
			int count = 0;
			for (int i = 0; i < (int)self.memory.all_events.size(); i++)
			{
				// if this resolves true, its an action 
				if (!self.memory.all_events.at(i).event_type)
				{
					//check if its the same action type
					if (self.memory.all_events.at(i).act.act_type == act_type)
					{
						count++;
					}
				}
			}
			double abundance = (double)count / self.memory.all_events.size();
			abundance = align1(abundance);
			return abundance;
		}

		// next up on list of things to do - implement a function for 
		// determining the "presedence" of an action when
		// compared to an experience
		// this is a measure of the distance between 
		// when the action happened and when the experience happened
		// restrictions - needs to resolve to 0 when the experience
		// happens BEFORE the action. it's to help measure how likely it is
		// that a given action in memory caused a given experience
		// this is primarily for use in the find_presedent_actions() function
		// which is in turn used by imagine() over in Act.h
		// ranges from 0 to 1

		// syntax: Action, Experience
		// warning: undefined behavior if not used properly
		static double get_presedence(Event a, Event e)
		{
			double presedence = 0;
			unsigned __int64 difference = 0;
			// true if a is an action and e is an experience
			if (!a.event_type && e.event_type)
			{
				if (a.time < e.time)
				{
					difference = e.time - a.time;
				}
			}
			presedence = align1((double)difference);
			return presedence;
		}

		static double get_collection_presedence(Collection c1, Collection c2)
		{
			double presedence = 0;
			unsigned __int64 difference = 0;
			// true if a is an action and e is an experience
			if (c1.time < c2.time)
			{
				difference = c2.time - c1.time;
			}
			else
			{
				return presedence;
			}
			presedence = align1((double)difference);
			return presedence;
		}

		// this function finds actions from memory
		// which have a high presedence compared
		// to the current experience, then returns a few
		// of the highest of them in a vector.
		// this might be resource intensive.
		static std::vector<Event> find_presedent_actions(Mind self, Event e)
		{
			double current_pres = 0;
			double prev_pres = 0;
			std::vector<Event> actions_list;
			std::vector<double> pres_list;
			std::vector<int> indexes_list;
			std::vector<Event> return_events;
			// true if its an experience
			if (e.event_type)
			{
				for (auto i : self.memory.all_events)
				{
					// true if we got an action
					if (!i.event_type)
					{
						actions_list.push_back(i);
					}
				}
				bool done = false;
				Event dummy;
				while (!done)
				{
					int count = 0;
					for (int i = 0; i < (int)actions_list.size(); i++)
					{
						current_pres = get_presedence(actions_list.at(i), e);
						if (current_pres > prev_pres)
						{
							if (i > 0)
							{
								dummy = actions_list[i];
								actions_list[i] = actions_list[i - 1];
								actions_list[i - 1] = dummy;
								prev_pres = current_pres;
								count++;
							}
						}
					}
					if (count == 0)
					{
						done = true;
					}
				}
			}
			else
			{
				std::cout << "Error: attempting to obtain presedent actions for an experience.";
				std::cout << std::endl;
				return return_events;
			}
			// lightly random choice of which actions get put into the final result
			// increasing tolerance decreases the randomness
			// dont let tolerance be <= 0 though
			if (actions_list.size() > 0)
			{
				double token = 1;
				int choice = 0;
				double tolerance = 0.3;

				for (int i = 0; i < (int)actions_list.size(); i++)
				{
					int n = 1 + i;
					int v = 1 + i + (int)(tolerance * (int)actions_list.size());
					double rand = (1 / double(n)) + (1 / double(mistake(1, v)));
					if (rand > token)
					{
						token = rand;
						return_events.push_back(actions_list.at(i));
					}
				}

			}
			return return_events;
		}

		// gives us a way of classifying 
		// different groups of actions
		static int get_action_group(int id)
		{

			// typing actions
			// associated with restlessness
			if (id < 42)
			{
				return 1;
			}
			// seeing actions
			// associated with curiosity
			else if (id < 56)
			{
				return 2;
			}
			// thinking actions
			// associated with questioning
			else if (id < 60)
			{
				return 3;
			}
			else
			{
				std::cout << "Error: attempted invalid action.\n";
				return 0;
			}
		}

		static Collection collection_search(Mind c, int id)
		{
			// search in active memory first
			for (int i = 0; i < (int)c.memory.all_collections.size(); i++)
			{
				if (id == c.memory.all_collections.at(i).index)
				{
					return c.memory.all_collections.at(i);
				}
			}

			// if its not there, get it out of storage
			return c.memory.storage.retrieve_collection(id);
		}

		static std::vector<double> calculate_relation_values(Relation r)
		{
			r.organize();
			std::vector<double> results;
			double correlation = 0;
			double similarity = 0;
			double value = 0;
			double presedence = 0;
			double relativity = 0;

			std::vector<double> presedences;
			int len = (int)r.collections.size() - 1;
			for (int i = 0; i < len; i++)
			{
				correlation += get_collection_correlation(
					r.collections.at(i), r.collections.at(i + 1));

				similarity += get_collection_similarity(
					r.collections.at(i), r.collections.at(i + 1));

				value += r.collections.at(i).value;

				presedence += get_collection_presedence(
					r.collections.at(i), r.collections.at(i + 1));
			}

			value += r.collections.back().value;

			correlation = correlation / len;

			similarity = similarity / len;

			value = value / len;

			presedence = presedence / len;

			double cor1 = 1 - correlation;
			double sim1 = 1 - similarity;
			double pres1 = 1 - presedence;

			if (cor1 <= 0.001)
			{
				cor1 = 0.001;
			}

			if (sim1 <= 0.001)
			{
				sim1 = 0.001;
			}

			if (pres1 <= 0.001)
			{
				pres1 = 0.001;
			}


			relativity = 1 / (cor1 * sim1 * pres1);
			relativity = align1(relativity);

			results.push_back(correlation);
			results.push_back(similarity);
			results.push_back(value);
			results.push_back(presedence);
			results.push_back(relativity);
			return results;
		}

		// find the average relativity found in all relations
		static double avg_collection_relativity(Mind self, Collection c)
		{
			int count = 0;
			double avg = 0;
			for (auto i : self.memory.all_relations)
			{
				for (auto j : i.indices)
				{
					if (c.index == j)
					{
						avg += i.relativity;
						count++;
					}
				}
			}
			avg = align1(avg / count);
			return avg;
		}

		// average together the feelings in each event in a collection
		static Feeling avg_collection_feelings(Collection c)
		{
			Feeling feel;
			int count = 0;
			for (auto i : c.events_vector)
			{
				feel.happiness += i.fln.happiness;
				feel.sadness += i.fln.sadness;
				feel.restlessness += i.fln.restlessness;
				feel.curiosity += i.fln.curiosity;
				feel.questioning += i.fln.questioning;
				feel.tiredness += i.fln.tiredness;
				feel.tolerance += i.fln.tolerance;
				count++;
			}
			feel.happiness = feel.happiness / count;
			feel.sadness = feel.sadness / count;
			feel.restlessness = feel.restlessness / count;
			feel.curiosity = feel.curiosity / count;
			feel.questioning = feel.questioning / count;
			feel.tiredness = feel.tiredness / count;
			feel.tolerance = feel.tolerance / count;

			return feel;
		}

		// checks a sequence to see if everything is going according to the plan
		static bool check_sequence(Mind self, Sequence s)
		{
			// check if there is a plan
			// and if anything has happened yet since
			// starting the plan
			if (s.execution.events_vector.empty())
			{
				return false;
			}
			if (s.plan.events_vector.empty())
			{
				return false;
			}
			Collection test;
			Collection test2;
			test.add_event(s.plan.events_vector.at(0));
			// TODO - fix this function so it properly finds
			// the previous collection created by existing
			test.add_event(s.execution.events_vector.at(0));
			test2.add_event(s.plan.events_vector.at(0));

			// check if things are going according to plan
			for (int i = 1; i < (int)s.plan.events_vector.size(); i++)
			{
				test2.add_event(s.plan.events_vector.at(i));
				test.add_event(s.plan.events_vector.at(i));
				test.add_event(s.execution.events_vector.at(i));
				double similarity = get_collection_similarity(test, test2);
				double upper = s.plan.similarity * (1 + self.tolerance);
				double lower = s.plan.similarity * (1 - self.tolerance);
				if (!test.experiences_vector.empty()
					&& !test2.experiences_vector.empty()
					&& similarity > lower
					&& similarity < upper)
				{
					self.tolerance = self.tolerance * 0.99;
					return true;
				}
				else
				{
					self.tolerance = self.tolerance * 1.01;
					return false;
				}
			}
			return true;
		}

		static void set_sequence_execution(Mind self, Sequence s)
		{
			if (self.memory.all_real_collections.size() > 0)
			{
				s.update_execution(self.memory.all_real_collections.back());
			}
			else
			{
				s.update_execution(self.third_thought);
			}
		}
	};

}
