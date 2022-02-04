#pragma once
#include <vector>
#include "utilities.h"
#include "data_types.h"
#include "Storage.h"

namespace ocb
{
	class Memory
	{
	public:
		Storage storage;

		std::vector<Collection> all_collections;
		std::vector<Collection> all_real_collections;
		std::vector<Collection> all_thoughts;
		std::vector<Collection> all_new_thoughts;
		std::vector<Relation> all_relations;
		std::vector<Relation> all_new_relations;
		std::vector<Event> all_events;
		std::vector<Event> all_actions;
		std::vector<Event> all_experiences;
		std::vector<int> actions_index;
		double avg_relativity;
		std::vector<Sequence> plans;
		Sequence current_plan;
		Collection newest_collection;
		Relation newest_relation;
		std::vector<char> all_action_names = { '1', '2', '3', '4', '5', '6', '7', '8', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '.', ',', '?', ' ', '\n' };


		Memory()
		{
			for (int i = 0; i < NUMBER_OF_ACTIONS; i++)
			{
				actions_index.push_back(i);
			}
			all_collections = {};
		}

		// sorts the memory according to how valuable each collection is
		// descending order
		void organize()
		{
			bool done = false;
			while (!done)
			{
				int count = 0;
				for (int i = 0; i < (int)all_collections.size() - 1; i++)
				{
					if (all_collections.at(i).importance < all_collections.at(i + 1).importance)
					{
						Collection dummy = all_collections.at(i);
						all_collections.at(i) = all_collections.at(i + 1);
						all_collections.at(i + 1) = dummy;
						count++;
					}

				}
				if (count == 0)
				{
					done = true;
				}
			}
		}

		// organize by descending relativity
		void organize_relations()
		{
			bool done = false;
			while (!done)
			{
				Relation dummy;
				int count = 0;
				for (int i = 0; i < (int)all_relations.size() - 1; i++)
				{
					if (all_relations.at(i).relativity > all_relations.at(i + 1).relativity)
					{
						dummy = all_relations.at(i);
						all_relations.at(i) = all_relations.at(i + 1);
						all_relations.at(i + 1) = dummy;
						count++;
					}

				}
				if (count == 0)
				{
					done = true;
				}
			}
		}

		void add_collection(Collection c)
		{
			all_collections.push_back(c);
		}

		void add_memory(Collection c)
		{
			all_real_collections.push_back(c);
			all_collections.push_back(c);
		}

		void add_relation(Relation r)
		{
			all_relations.push_back(r);
		}

		void add_new_relation(Relation r)
		{
			all_new_relations.push_back(r);
			add_relation(r);
			newest_relation = r;
		}

		// imagine() from Act.h adds to here
		void add_thought(Collection c)
		{
			all_thoughts.push_back(c);
			all_collections.push_back(c);
			newest_collection = c;
		}

		void add_new_thought(Collection c)
		{
			all_new_thoughts.push_back(c);
			add_thought(c);
		}

		void add_event(Event e)
		{
			all_events.push_back(e);
			if (e.event_type)
			{
				all_experiences.push_back(e);
			}
			else
			{
				all_actions.push_back(e);
			}
		}

		// think im not using this yet
		void set_avg_relativity()
		{
			int count = 0;
			double sum = 0;
			for (auto i : all_relations)
			{
				sum += i.relativity;
				count++;
			}
			if (count + 0)
			{
				avg_relativity = sum / count;
			}
		}

		void clear_memory()
		{
			all_collections = {};
			all_real_collections = {};
			all_thoughts = {};
			all_new_thoughts = {};
			all_relations = {};
			all_new_relations = {};
			all_events = {};
			all_actions = {};
			all_experiences = {};
			actions_index = {};
			avg_relativity = 0;
			plans = {};
		}

		void init_memory()
		{
			// add the most important collections from storage
			std::ifstream file;
			std::string line;
			double tolerance = 0.5;
			int runs = tolerance * storage.read_unify();
			std::cout << "runs - ";
			std::cout << runs;
			std::cout << std::endl;
			std::vector<int> indexes;
			std::vector<int> r_indexes;
			for (int i = 0; i < runs; i++)
			{
				file.open(storage.directory + storage.organized_records);
				{
					if (file.is_open())
					{
						std::getline(file, line);
						int len = std::stoi(line);
						for (int j = 0; j < len; j++)
						{
							std::getline(file, line);
							line = storage.remove_nl(line);
							indexes.push_back(std::stoi(line));
							std::getline(file, line);
							std::cout << std::endl;
						}
						file.close();
					}
					else
					{
						std::cout << "Error: unable to open collections organized record file from memory.";
					}
				}
				std::cout << "collections indexes size - ";
				std::cout << indexes.size();
				std::cout << std::endl;
				file.open(storage.directory + storage.relations_organized);
				{
					if (file.is_open())
					{
						std::getline(file, line);
						int len = std::stoi(line);
						for (int j = 0; j < len; j++)
						{
							std::getline(file, line);
							line = storage.remove_nl(line);
							r_indexes.push_back(std::stoi(line));
							std::getline(file, line);
							std::cout << std::endl;
						}
						file.close();
					}
					else
					{
						std::cout << "Error: unable to open organized relations record file.";
					}
				}
				std::cout << "relations indexes size - ";
				std::cout << r_indexes.size();
				std::cout << std::endl;
			}
			for (int i = 0; i < indexes.size(); i++)
			{
				Collection newc = storage.retrieve_collection(indexes.at(i));
				for (auto j : newc.events_vector)
				{
					all_events.push_back(j);
				}
				std::cout << "adding collection at index: ";
				std::cout << newc.index;
				std::cout << std::endl;
				all_collections.push_back(newc); 
				if (i < (int)r_indexes.size())
				{
					all_relations.push_back(storage.retrieve_relation(r_indexes.at(i)));
				}
				/*
				else
				{
					std::cout << "Error: unable to add any collections to memory during initialization.";
					std::cout << std::endl;
				}
				if (i < (int)r_indexes.size())
				{
					all_relations.push_back(storage.retrieve_relation(r_indexes.at(i)));
				}
				else
				{
					std::cout << "Error: unable to add any relations to memory during initialization.";
					std::cout << std::endl;
				}
				*/
			}
		}

		void create_plan(int id)
		{
			Sequence plan;
			plan.init_sequence(all_collections.at(id));
			plan.set_index(id);
			plans.push_back(plan);
		}
	};

}
