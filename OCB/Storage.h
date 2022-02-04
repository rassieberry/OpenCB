#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <opencv2/imgcodecs.hpp>
#include <Windows.h>
#include <stdlib.h>
#include <filesystem>

#include "utilities.h"
#include "data_types.h"

#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

namespace ocb
{

	class Storage
	{
	public:
		std::string directory;
		std::string collections_folder = "\\collections\\";
		std::string records_folder = "\\collections\\records\\";
		std::string unifying_record = "\\collections\\records\\unify.txt";
		std::string organized_records = "\\collections\\records\\organized.txt";
		std::string records_index = "\\collections\\records\\records_index.txt";

		std::string actions_folder = "\\events\\actions\\";
		std::string experiences_folder = "\\events\\experiences\\";
		std::string feelings_folder = "\\events\\feelings\\";
		std::string images_folder = "\\events\\experiences\\images\\";
		std::string events_records_folder = "\\events\\records\\";
		std::string events_unify = "\\events\\records\\events_unify.txt";

		std::string relations_folder = "\\relations\\";
		std::string relations_records = "\\relations\\records\\";
		std::string relations_unify = "\\relations\\records\\unify.txt";
		std::string relations_organized = "\\relations\\records\\organized.txt";
		std::string relations_records_index = "\\relations\\records\\relations_records_index.txt";

		//bf::path full_path(bf::current_path());
		//bf::path collections(full_path / collections_folder);
		//bf::path collections_records(full_path / records_folder);
		//bf::path collections_unify(full_path / unifying_record);
		//bf::path collections_organized(full_path / organized_records);
		//bf::path collections_records_index(full_path / records_index);

		//bf::path actions(full_path / actions_folder);
		//bf::path experiences(full_path / experiences_folder);
		//bf::path images(full_path / images_folder);

		//bf::path relations(full_path / relations_folder);
		//bf::path relations_records(full_path / relations_records);
		//bf::path relations_unify(full_path / relations_unify);
		//bf::path relations_organized(full_path / relations_organized);
		//bf::path relations_records_index(full_path / relations_records_index);

		Storage()
		{
			char base[MAX_FILEPATH] = { 0 };
			DWORD base_length = MAX_FILEPATH;
			DWORD z = GetModuleFileNameA(NULL, base, base_length);
			directory = std::string(base);
			std::string exec_name = "/OCB.exe";
			for (int i = 0; i < exec_name.length(); i++)
			{
				directory.pop_back();
			}
			
			std::filesystem::create_directories(directory + collections_folder);
			std::filesystem::create_directories(directory + records_folder);
			//std::filesystem::create_directories(directory + unifying_record);
			//std::filesystem::create_directories(directory + organized_records);
			//std::filesystem::create_directories(directory + records_index);

			std::filesystem::create_directories(directory + actions_folder);
			std::filesystem::create_directories(directory + experiences_folder);
			std::filesystem::create_directories(directory + feelings_folder);
			std::filesystem::create_directories(directory + images_folder);
			std::filesystem::create_directories(directory + events_records_folder);
			//std::filesystem::create_directories(directory + events_unify);

			std::filesystem::create_directories(directory + relations_folder);
			std::filesystem::create_directories(directory + relations_records);
			//std::filesystem::create_directories(directory + relations_unify);
			//std::filesystem::create_directories(directory + relations_organized);
			//std::filesystem::create_directories(directory + relations_records_index);
			
			/*
			if (!dir.exists(path12))
			{
				std::cout << "Creating directories...\n";
				dir.mkpath(path);
				dir.mkpath(path2);

				dir.mkpath(path6);
				dir.mkpath(path7);
				dir.mkpath(path8);
				dir.mkpath(path9);
				dir.mkpath(path10);

				dir.mkpath(path12);
				dir.mkpath(path13);
			}
			else
			{
				std::cout << "Error: Could not create sub-directories. Do they already exist?\n";
			}
			*/
			std::ifstream file;
			file.open(directory + records_index);
			if (!file.is_open())
			{
				init_unify();
				init_events_unify();
				init_relations_unify();
				init_records_index();
				init_relations_records_index();
			}
			file.close();
		}

		std::string remove_nl(std::string str)
		{
			str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
			return str;
		}

		bool file_empty(std::ifstream& file)
		{
			return file.peek() == std::ifstream::traits_type::eof();
		}

		// so what we're trying to do here 
		// is to store and process the long term memories of the program
		// on the harddrive. we've got to store them in a location that can 
		// be accessed somehow, and we've got to organize them 
		// somehow as well in order to pass the most important and relevant
		// memories back to the system memory for use in the short term memory

		// now, first, we need to store everything in long term memory
		// then we need to organize it so that the most important good
		// memories are on top and the most important bad memories are on the bottom

		// then we need to implement a way for the short term memory to get long term 
		// storage memories out from storage for use in the program
		// this needs to happen in real time and it needs to be useful 
		// and fast. im not sure how that function will work yet.

		// here, we need to store each collection with the filename being its unique 
		// index. it also needs to have its important variables stored in a text file
		// that identifies them by its index, so that they can be organized by those variables
		void store_collection(Collection c)
		{
			std::ofstream file;
			file.open(directory + collections_folder + std::to_string(c.index) + ".txt");
			if (file.is_open())
			{
				if (true)
				{
					file << std::to_string(c.index);
					file << std::endl;
					file << std::to_string(c.time);
					file << std::endl;
					file << std::to_string(c.value);
					file << std::endl;
					file << std::to_string(c.correlation);
					file << std::endl;
					file << std::to_string(c.similarity);
					file << std::endl;
					file << std::to_string(c.importance);
					file << std::endl;
					file << std::to_string(c.events_vector.size());
					file << std::endl;
					for (auto i : c.events_vector)
					{
						file << std::to_string(i.index);
						file << std::endl;
						if (i.event_type)
						{
							file << "true";
							file << std::endl;
						}
						else
						{
							file << "false";
							file << std::endl;
						}
						store_event(i);
					}


					file.close();
				}
				else
				{
					std::cout << ("Error writing to collection file: " + std::to_string(c.index) + " File already exists.\n");
					std::cout << std::endl;
				}
			}
			else
			{
				std::cout << "Error opening collection file: " + c.index + '\n';
				std::cout << std::endl;
			}
			file.close();
			file.open(directory + records_folder + std::to_string(c.index) + ".txt");
			if (file.is_open())
			{
				file << std::to_string(c.index);
				file << std::endl;
				file << std::to_string(c.importance);
				file << std::endl;
				incr_records_index();
			}
		}

		// we store events the same way, in a seperate folder, each event file named after its
		// unique index. actions and experiences are stored seperately
		void store_event(Event e)
		{
			std::ofstream file;
			if (e.event_type)
			{
				file.open(directory + experiences_folder + std::to_string(e.index) + ".txt");
			}
			else
			{
				file.open(directory + actions_folder + std::to_string(e.index) + ".txt");

			}
			if (file.is_open())
			{
				file << e.index;
				file << std::endl;
				file << e.time;
				file << std::endl;
				if (!e.event_type)
				{
					file << "false";
					file << std::endl;
					file << e.act.act_type;
					file << std::endl;
				}
				else
				{
					file << "true";
					file << std::endl;
					file << e.act.collections_indices.size();
					file << std::endl;
					for (int i = 0; i < IMG_DIVISIONS; i++)
					{
						std::string img_path = directory + images_folder + std::to_string(e.index) + "-" + std::to_string(i) + ".jpg";
						cv::imwrite(img_path, e.exp.this_image.at(i));
					}

				}
				file.close();
				store_feeling(e.fln, e.index);
			}
			else
			{
				std::cout << "Error opening event file: " + std::to_string(e.index) + '\n';
				std::cout << std::endl;
			}
		}

		void store_feeling(Feeling f, int id)
		{
			std::ofstream file;
			file.open(directory + feelings_folder + std::to_string(id) + ".txt");
			if (file.is_open())
			{
				file << f.happiness;
				file << std::endl;
				file << f.sadness;
				file << std::endl;
				file << f.restlessness;
				file << std::endl;
				file << f.curiosity;
				file << std::endl;
				file << f.questioning;
				file << std::endl;
				file << f.tiredness;
				file << std::endl;
				file << f.tolerance;
				file << std::endl;
			}
			else
			{
				std::cout << "Error: could not store feeling file at index: " + id + '\n';
				std::cout << std::endl;
			}
		}

		// next we need a way to build a collection and an event from their 
		// text file storage. during this we'll need to retrieve each event
		// that was part of the collection in memory from its location
		// in the events folder

		Collection retrieve_collection(int id)
		{
			Collection newc;
			std::ifstream file;
			file.open(directory + collections_folder + std::to_string(id) + ".txt");
			if (file.is_open())
			{
				int num_lines = 0;
				std::string line;
				std::string text;
				if (!file_empty(file))
				{
					std::getline(file, line);
					line = remove_nl(line);
					newc.index = std::stoi(line);

					std::getline(file, line);
					line = remove_nl(line);
					newc.time = std::stoull(line);

					std::getline(file, line);
					line = remove_nl(line);
					newc.value = std::stod(line);

					std::getline(file, line);
					line = remove_nl(line);
					newc.correlation = std::stod(line);

					std::getline(file, line);
					line = remove_nl(line);
					newc.similarity = std::stod(line);

					std::getline(file, line);
					line = remove_nl(line);
					newc.importance = std::stod(line);

					std::getline(file, line);
					line = remove_nl(line);
					int events_vect_size = std::stoi(line);
					for (int i = 0; i < events_vect_size; i++)
					{
						Event new_event;
						std::getline(file, line);
						line = remove_nl(line);
						int new_id = std::stoi(line);

						std::getline(file, line);
						line = remove_nl(line);
						bool type;
						if (line == "true")
						{
							type = true;
						}
						else
						{
							type = false;
						}/*
						std::cout << "retrieving event at index: ";
						std::cout << new_id;
						std::cout << std::endl;*/
						new_event = retrieve_event(new_id, type);
						newc.add_event(new_event);
					}
				}
				else
				{
					std::cout << "Error: tried to retrieve an empty collection file at index: " + id + '\n';
					std::cout << std::endl;
				}
				file.close();
			}
			else
			{
				std::cout << "Error opening a collection file to retrieve at index: " + id + '\n';
				std::cout << std::endl;
			}
			return newc;
		}

		Event retrieve_event(int id, bool e_type)
		{
			Event new_event;
			new_event.fln = retrieve_feeling(id);
			std::ifstream file;
			std::string line;
			if (e_type)
			{
				file.open(directory + experiences_folder + std::to_string(id) + ".txt");
				if (file.is_open())
				{
					std::getline(file, line);
					line = remove_nl(line);
					new_event.index = std::stoi(line);

					std::getline(file, line);
					line = remove_nl(line);
					new_event.time = (std::stoull(line));

					std::getline(file, line);
					line = remove_nl(line);
					bool type;
					// this stuff is messed up but working for now
					// i think
					if (line == "true")
					{
						type = true;
					}
					else
					{
						type = false;
					}
					new_event.exp.is_feeling = false;

					if (!new_event.exp.is_feeling)
					{
						std::getline(file, line);
						line = remove_nl(line);
						new_event.event_type = e_type;
						Experience new_exp;

						for (int i = 0; i < IMG_DIVISIONS; i++)
						{
							std::string img_path = directory + images_folder + std::to_string(id) + "-" + std::to_string(i) + ".jpg";
							cv::Mat temp_img = cv::imread(img_path);
							new_exp.this_image.push_back(temp_img);
						}
						new_event.set_experience(new_exp);
						file.close();
					}
					else
					{/*
						Experience new_exp;
						std::getline(file, line);
						line = remove_nl(line);
						new_exp.happiness = std::stod(line);
						std::getline(file, line);
						line = remove_nl(line);
						new_exp.sadness = std::stod(line);
						std::getline(file, line);
						line = remove_nl(line);
						new_exp.restlessness = std::stod(line);
						std::getline(file, line);
						line = remove_nl(line);
						new_exp.curiosity = std::stod(line);
						std::getline(file, line);
						line = remove_nl(line);
						new_exp.questioning = std::stod(line);
						std::getline(file, line);
						line = remove_nl(line);
						new_exp.tiredness = std::stod(line);
						new_event.set_experience(new_exp);*/
					}
				}
				else
				{
					std::cout << "Error: unable to open experience file at index: " + std::to_string(id);
					std::cout << std::endl;
				}
				file.close();

			}
			else
			{
				file.open(directory + actions_folder + std::to_string(id) + ".txt");
				if (file.is_open())
				{
					std::getline(file, line);
					line = remove_nl(line);
					new_event.index = std::stoi(line);

					std::getline(file, line);
					line = remove_nl(line);
					new_event.time = (std::stoull(line));

					std::getline(file, line);
					line = remove_nl(line);
					new_event.event_type = e_type;

					Action new_act;
					std::getline(file, line);
					line = remove_nl(line);
					new_act.set_act_type(std::stoi(line));
					file.close();
					new_event.set_action(new_act);
				}
				else
				{
					std::cout << "Error: unable to open action file at index: " + std::to_string(id);
					std::cout << std::endl;
				}
			}
			return new_event;
		}

		Feeling retrieve_feeling(int id)
		{
			Feeling f;
			std::ifstream file;
			std::string line;
			file.open(directory + feelings_folder + std::to_string(id) + ".txt");
			if (file.is_open())
			{
				std::getline(file, line);
				line = remove_nl(line);
				f.happiness = std::stod(line);

				std::getline(file, line);
				line = remove_nl(line);
				f.sadness = std::stod(line);

				std::getline(file, line);
				line = remove_nl(line);
				f.restlessness = std::stod(line);

				std::getline(file, line);
				line = remove_nl(line);
				f.curiosity = std::stod(line);

				std::getline(file, line);
				line = remove_nl(line);
				f.questioning = std::stod(line);

				std::getline(file, line);
				line = remove_nl(line);
				f.tiredness = std::stod(line);

				std::getline(file, line);
				line = remove_nl(line);
				f.tolerance = std::stod(line);
			}
			return f;
		}

		// we need to write each new collection that gets put in storage
		// into a text file with its index, value, and math
		// we'll use these entries to organize the storage later

		void record_collections(std::vector<Collection> v)
		{
			for (auto i : v)
			{
				store_collection(i);
			}
		}

		// in order to record collections though, we need a unifying record that always exists 
		// which indexes all the records in order of their creation
		// each record is going to have a bunch of collections indexed in it

		int read_unify()
		{
			std::ifstream file;
			std::string line;
			int latest = 0;
			file.open(directory + unifying_record);
			if (file.is_open())
			{
				std::getline(file, line);
				line = remove_nl(line);
				latest = std::stoi(line);
				return latest;
			}
			else
			{
				std::cout << "test5-9";
				std::cout << std::endl;
				std::cout << "Error: unable to read unifying record file" + '\n';
				std::cout << std::endl;
			}
			file.close();
			return latest;
		}

		void incr_unify()
		{
			std::ifstream file;
			std::string line;
			int latest = 0;
			file.open(directory + unifying_record);
			if (file.is_open())
			{
				std::getline(file, line);
				line = remove_nl(line);
				latest = std::stoi(line);
				latest += 1;
				file.close();
				std::ofstream ofile;
				ofile.open(directory + unifying_record);
				ofile << latest;
				ofile.close();
			}
			else
			{
				std::cout << "Error: unable to increment unifying record file" + '\n';
				std::cout << std::endl;
			}
		}

		void init_unify()
		{
			std::ofstream file;
			std::string line;
			int latest = 0;
			file.open(directory + unifying_record);
			if (file)
			{
				file << '0';
				file.close();
			}
			else
			{
				std::cout << "Error: unable to create unifying record file" + '\n';
				std::cout << std::endl;
			}
		}

		int read_records_index()
		{
			std::ifstream file;
			std::string line;
			int latest = -1;
			file.open(directory + records_index);
			if (file.is_open())
			{
				std::getline(file, line);
				line = remove_nl(line);
				latest = std::stoi(line);
				file.close();
			}
			else
			{
				std::cout << "Error: unable to read records index file\n";
				std::cout << std::endl;
			}
			return latest;
		}

		void incr_records_index()
		{
			std::ifstream file;
			std::ofstream file2;
			std::string line;
			int latest = 0;
			file.open(directory + records_index);
			if (file.is_open())
			{
				std::getline(file, line);
				line = remove_nl(line);
				latest = std::stoi(line);
				latest += 1;
				file.close();
				file2.open(directory + records_index);
				file2 << latest;
				file2.close();
			}
			else
			{
				std::cout << "Error: unable to increment records index file\n";
				std::cout << std::endl;
			}
		}

		void init_records_index()
		{
			std::ofstream file;
			std::string line;
			int latest = 0;
			file.open(directory + records_index);
			if (file.is_open())
			{
				file << '0';
				file.close();
			}
			else
			{
				std::cout << "Error: unable to create records index file\n";
				std::cout << std::endl;
			}
		}


		void store_relation(Relation r)
		{
			std::ofstream file;
			file.open(directory + relations_folder + std::to_string(r.ind) + ".txt");
			if (file.is_open())
			{
				if (true)
				{
					file << r.ind;
					file << std::endl;
					file << r.correlation;
					file << std::endl;
					file << r.similarity;
					file << std::endl;
					file << r.value;
					file << std::endl;
					file << r.presedence;
					file << std::endl;
					file << r.relativity;
					file << std::endl;
					file << r.indices.size();
					file << std::endl;
					for (auto i : r.indices)
					{
						file << i;
						file << std::endl;
					}
				}
				file.close();
			}
			else
			{
				std::cout << "Error: unable to open relation file at index: " + r.ind + '\n';
				std::cout << std::endl;
			}

			int record_index = read_relations_records_index();
			std::ofstream record;
			record.open(directory + relations_records + std::to_string(record_index) + ".txt");
			if (record.is_open())
			{
				record << r.ind;
				record << std::endl;
				record << r.value;
				record << std::endl;
				record.close();
				incr_relations_records_index();
			}
			else
			{
				std::cout << "Error: could not open relations records file at: " + record_index + '\n';
				std::cout << std::endl;
			}
		}

		Relation retrieve_relation(int id)
		{

			Event new_event;
			std::ifstream file;
			std::string line;

			file.open(directory + relations_folder + std::to_string(id) + ".txt");
			if (file.is_open())
			{
				Relation r;
				std::getline(file, line);
				r.set_index(std::stoi(line));
				std::getline(file, line);
				r.ind = std::stoi(line);
				std::getline(file, line);
				r.correlation = std::stoi(line);
				std::getline(file, line);
				r.similarity = std::stod(line);
				std::getline(file, line);
				r.value = std::stod(line);
				std::getline(file, line);
				r.presedence = std::stod(line);
				std::getline(file, line);
				r.relativity = std::stod(line);
				std::getline(file, line);
				int len = std::stoi(line);
				for (int i = 0; i < len; i++)
				{
					std::getline(file, line);
					r.indices.push_back(std::stod(line));
				}
				file.close();
				return r;
			}
			else
			{
				std::cout << "Error: could not open relation file at index: " + id + '\n';
				std::cout << std::endl;
				Relation r;
				return r;
			}
		}
		int read_relations_unify()
		{
			std::ifstream file;
			std::string line;
			int latest = -1;
			file.open(directory + relations_unify);
			if (file.is_open())
			{
				std::getline(file, line);
				line = remove_nl(line);
				latest = std::stoi(line);
				file.close();
			}
			else
			{
				std::cout << "Error: unable to read relations unifying record file\n";
				std::cout << std::endl;
			}
			return latest;
		}

		void incr_relations_unify()
		{
			std::ifstream file;
			std::ofstream file2;
			std::string line;
			int latest = 0;
			file.open(directory + relations_unify);
			if (file.is_open())
			{
				std::getline(file, line);
				line = remove_nl(line);
				latest = std::stoi(line);
				latest += 1;
				file.close();
				file2.open(directory + relations_unify);
				file2 << latest;
				file2.close();
			}
			else
			{
				std::cout << "Error: unable to increment relations unifying record file\n";
				std::cout << std::endl;
			}
		}

		void init_relations_unify()
		{
			std::ofstream file;
			std::string line;
			int latest = 0;
			file.open(directory + relations_unify);
			if (file.is_open())
			{
				file << '0';
				file.close();
			}
			else
			{
				std::cout << "Error: unable to create unifying record file\n";
				std::cout << std::endl;
			}
		}

		void record_relations(std::vector<Relation> v)
		{
			int record_index = read_relations_records_index();
			incr_relations_records_index();
			std::ofstream record;
			record.open(directory + relations_records + std::to_string(record_index) + ".txt");
			if (record.is_open())
			{
				record << v.size();
				record << std::endl;
				for (auto i : v)
				{
					record << i.ind;
					record << std::endl;
					record << i.relativity;
					record << std::endl;
					store_relation(i);
				}
				record.close();
			}
			else
			{
				std::cout << "Error: could not open relations records file at: " + record_index + '\n';
				std::cout << std::endl;
			}
		}

		int read_relations_records_index()
		{
			std::ifstream file;
			std::string line;
			int latest = -1;
			file.open(directory + relations_records_index);
			if (file.is_open())
			{
				std::getline(file, line);
				line = remove_nl(line);
				latest = std::stoi(line);
				file.close();
			}
			else
			{
				std::cout << "Error: unable to read relations_records_index file\n";
				std::cout << std::endl;
			}
			return latest;
		}

		void incr_relations_records_index()
		{
			std::ifstream file;
			std::ofstream file2;
			std::string line;
			int latest = 0;
			file.open(directory + relations_records_index);
			if (file.is_open())
			{
				std::getline(file, line);
				line = remove_nl(line);
				latest = std::stoi(line);
				latest += 1;
				file.close();
				file2.open(directory + relations_records_index);
				file2 << latest;
				file2.close();
			}
			else
			{
				std::cout << "Error: unable to increment relations_records_index index file\n";
				std::cout << std::endl;
			}
		}

		void init_relations_records_index()
		{
			std::ofstream file;
			std::string line;
			int latest = 0;
			file.open(directory + relations_records_index);
			if (file.is_open())
			{
				file << '0';
				file.close();
			}
			else
			{
				std::cout << "Error: unable to create relations_records_index index file\n";
				std::cout << std::endl;
			}
		}

		// here, we need a way of organizing each collection's place in memory storage
		// according to certain parameters like value, relevance, etc.
		// for instance, if we want the 5 most valuable memories, we should be able to get those
		// pretty quickly, but if we want a memory from a long time ago which doesn't
		// have much value, either positive or negative, then that should 
		// take longer to find. So memories should rise to the top based on value
		// and memories which have low value and are far in the past should be forgotten 
		// or hard to find.

		// the way this is gonna work is we'll have a text file of some kind which
		// stores the index, value, and correlation of each collection in storage.
		// then, when the system wants to get a few important memories out, it'll 
		// start at the top of that text file and go down the list until it finds 
		// all the collections it wants and returns them.

		// this function will create that text file and organize it in terms of value
		// and correlation in descending order
		void organize_storage()
		{
			std::vector<int> indexes;
			std::vector<double> importances;
			int max_index = 0;
			std::string line;
			std::ifstream file;
			std::ofstream file2;
			file.open(directory + records_index);
			if (file.is_open())
			{
				// get which record index we're on
				std::getline(file, line);
				line = remove_nl(line);
				max_index = std::stoi(line);
				file.close();
			}
			else
			{
				std::cout << "Error: unable to open record index file\n";
				std::cout << std::endl;
			}
			for (int i = 0; i < max_index; i++)
			{
				// add all the data from the records into the three
				// vectors we need
				file.open(directory + records_folder + std::to_string(i) + ".txt");
				{
					if (file.is_open())
					{
						std::getline(file, line);
						line = remove_nl(line);
						indexes.push_back(std::stoi(line));
						std::getline(file, line);
						line = remove_nl(line);
						importances.push_back(std::stod(line));
						file.close();
					}
					else
					{
						std::cout << "Error: could not open records file at index: " + i + '\n';
						std::cout << std::endl;
					}
				}
			}

			// organize those vectors in descending order of how we want it
			int dummy = 0;
			double dummy2 = 0;
			for (int i = 0; i < (int)indexes.size() - 1; i++)
			{
				if (importances[i] < importances[i + 1])
				{
					dummy = indexes[i];
					indexes[i] = indexes[i + 1];
					indexes[i + 1] = dummy;
					dummy2 = importances[i];
					importances[i] = importances[i + 1];
					importances[i + 1] = dummy2;
				}
			}
			file2.open(directory + organized_records);
			{
				if (file2.is_open())
				{
					std::cout << std::endl;
					file2 << indexes.size();
					file2 << std::endl;
					for (int i = 0; i < indexes.size(); i++)
					{
						file2 << indexes[i];
						file2 << std::endl;
						file2 << importances[i];
						file2 << std::endl;
					}
					file2.close();
				}
			}

			// now we do the same for relations
			std::vector<int> r_indexes;
			std::vector<double> relativities;
			int max_r_index = 0;
			file.open(directory + relations_records_index);
			if (file.is_open())
			{
				// get which relations record index we're on
				std::getline(file, line);
				line = remove_nl(line);
				max_r_index = std::stoi(line);
				file.close();
			}
			else
			{
				std::cout << "Error: could not open relations unifying file.\n";
				std::cout << std::endl;
			}
			for (int i = 0; i < max_r_index; i++)
			{
				// add all the data from the records into the two
				// vectors we need
				file.open(directory + relations_organized + std::to_string(i) + ".txt");
				{
					if (file.is_open())
					{
						std::cout << std::endl;
						std::getline(file, line);
						line = remove_nl(line);
						int len = std::stoi(line);
						for (int j = 0; j < len; j++)
						{
							std::getline(file, line);
							line = remove_nl(line);
							r_indexes.push_back(std::stoi(line));
							std::getline(file, line);
							line = remove_nl(line);
							relativities.push_back(std::stod(line));
						}
						file.close();
					}
					else
					{
						std::cout << "Error: could not open records file at index: " + i + '\n';
						std::cout << std::endl;
					}
				}
			}
			// organize those vectors in descending order of how we want it
			int dummy3 = 0;
			double dummy4 = 0;
			for (int i = 0; i < (int)r_indexes.size() - 1; i++)
			{
				if (relativities[i] < relativities[i + 1])
				{
					dummy3 = r_indexes[i];
					r_indexes[i] = r_indexes[i + 1];
					r_indexes[i + 1] = dummy3;
					dummy4 = relativities[i];
					relativities[i] = relativities[i + 1];
					relativities[i + 1] = dummy4;
				}
			}
			file2.open(directory + relations_organized, std::ofstream::trunc);
			file2.close();
			file2.open(directory + relations_organized);
			{
				if (file2.is_open())
				{
					file2 << r_indexes.size();
					file2 << std::endl;
					for (int i = 0; i < r_indexes.size(); i++)
					{
						file2 << r_indexes[i];
						file2 << std::endl;
						file2 << relativities[i];
						file2 << std::endl;
					}
					file2.close();
				}
			}
		}

		std::vector<int> read_organized_records()
		{
			std::vector<int> records;
			std::ifstream file;
			std::string line;
			file.open(directory + organized_records);
			if (file.is_open())
			{
				std::cout << std::endl;
				getline(file, line);
				int len = std::stoi(line);
				for (int i = 0; i < len; i++)
				{
					getline(file, line);
					records.push_back(std::stoi(line));
					getline(file, line);
				}
			}
			else
			{
				std::cout << "Error: unable to open collections organized records file.\n";
				std::cout << std::endl;
			}
			return records;
		}

		std::vector<int> read_organized_relations()
		{
			std::vector<int> records;
			std::ifstream file;
			std::string line;
			file.open(directory + relations_organized);
			if (file.is_open())
			{
				std::cout << std::endl;
				getline(file, line);
				int len = std::stoi(line);
				for (int i = 0; i < len; i++)
				{
					getline(file, line);
					records.push_back(std::stoi(line));
					getline(file, line);
				}
			}
			else
			{
				std::cout << "Error: unable to open relations organized records file.\n";
				std::cout << std::endl;
			}
			return records;
		}

		int read_events_unify()
		{
			std::ifstream file;
			std::string line;
			int latest = 0;
			file.open(directory + events_unify);
			if (file.is_open())
			{
				std::getline(file, line);
				line = remove_nl(line);
				latest = std::stoi(line);
				file.close();
			}
			else
			{
				std::cout << "Error: unable to read events_unify record file\n";
				std::cout << std::endl;
			}
			return latest;
		}

		void incr_events_unify()
		{
			std::ifstream file;
			std::string line;
			int latest = 0;
			file.open(directory + events_unify);
			if (file.is_open())
			{
				std::getline(file, line);
				line = remove_nl(line);
				latest = std::stoi(line);
				latest += 1;
				file.close();
			}
			else
			{
				std::cout << "Error: unable to open events_unify record file\n";
				std::cout << std::endl;
			}
			std::ofstream file2;
			file2.open(directory + events_unify);
			if (file2.is_open())
			{
				std::cout << "latest in events_unify: ";
				std::cout << latest;
				std::cout << std::endl;

				file2 << latest;
				file2.close();
			}
			else
			{
				std::cout << "Error: unable to increment events_unify record file\n";
				std::cout << std::endl;
			}
		}

		void init_events_unify()
		{
			std::ofstream file;
			std::string line;
			int latest = 0;
			file.open(directory + events_unify);
			if (file)
			{
				file << '0';
				file << std::endl;
				file.close();
			}
			else
			{
				std::cout << "Error: unable to create events_unify record file\n";
				std::cout << std::endl;
			}
		}
	};

}
