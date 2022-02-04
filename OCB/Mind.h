#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/mat.hpp>

#include "utilities.h"
#include "data_types.h"
#include "Memory.h"

namespace ocb
{

	class Mind
	{
	public:
		Memory memory;
		Collection first_thought;
		Collection second_thought;
		Collection third_thought;
		Collection fourth_thought;

		Event first_event;
		Experience first_experience;

		Event second_event;
		Experience second_experience;

		Event fourth_event;
		Action last_action;

		Vision eye1;
		Time t;

		// feelings
		// these determine the tendency to 
		// do certain kinds of actions
		double happiness;
		double sadness;
		double curiosity;
		double restlessness;
		double questioning;
		double tiredness;
		double tolerance;

		// these are kind of like personality traits
		// you can modify these but make sure they 
		// are always x > 0 and x <= 1
		double happy_tolerance;
		double sad_tolerance;
		double curious_tolerance;
		double rest_tolerance;
		double quest_tolerance;
		double tired_tolerance;
		double tol_tolerance;

		double value_diff;
		int value_count;

		Mind(bool turn_on)
		{
			std::cout << "Initializing brain...";
			std::cout << std::endl;
			// current feelings values
			// start em at 0
			happiness = 0;
			sadness = 0;
			curiosity = 0;
			restlessness = 0;
			questioning = 0;
			tiredness = 0;
			tolerance = 1;

			// if you're going to modify these
			// make sure they arent zero and 
			// are less than or equal to 1
			// default values are just 1
			happy_tolerance = 1;
			sad_tolerance = 0.7;
			curious_tolerance = 0.9;
			rest_tolerance = 0.5;
			quest_tolerance = 1;
			tired_tolerance = 0.5;
			tol_tolerance = 0.8;

			value_diff = 0;
			value_count = 0;

			std::vector<double> feelings;
			feelings.push_back(happiness);
			feelings.push_back(sadness);
			feelings.push_back(restlessness);
			feelings.push_back(curiosity);
			feelings.push_back(questioning);
			feelings.push_back(tiredness);
			feelings.push_back(tolerance);
			// here's where we're putting the visual seeds
			// these images determine what the program 
			// likes and hates.
			// its the basis of the formation of attraction
			// and repulsion that forms the core of the
			// program's ability to think
			// i'll add more of these in the future
			std::string first_image_path = "happy3.jpg";
			std::string second_image_path = "despair.jpg";


			// everything here is just initializing the 
			// visual seed and action seed memories
			cv::Mat image = cv::imread(first_image_path);
			if (!image.data)
			{
				std::cout << ("Image 1 failed to load.\n");
			}
			else
			{
				first_experience.this_image = Image::process_image(
					Image::grayscale_file(
						first_image_path));
			}
			first_event.time = t.get_current_time();
			first_event.set_experience(first_experience);
			first_event.set_index(memory.storage.read_events_unify());
			memory.storage.incr_events_unify();
			Feeling first_feel;
			first_feel.set_feelings(feelings);
			first_event.set_feeling(first_feel);
			first_thought.add_event(first_event);
			memory.add_event(first_event);

			cv::Mat image2 = cv::imread(second_image_path);
			if (!image2.data)
			{
				std::cout << ("Image 2 failed to load.\n");
			}
			else
			{
				second_experience.this_image = Image::process_image(
					Image::grayscale_file(
						second_image_path));
			}
			second_event.time = t.get_current_time();
			second_event.set_experience(second_experience);
			second_event.set_index(memory.storage.read_events_unify());
			memory.storage.incr_events_unify();
			Feeling second_feel;
			second_feel.set_feelings(feelings);
			second_event.set_feeling(second_feel);
			second_thought.add_event(second_event);
			memory.add_event(second_event);

			for (int i = 0; i < NUMBER_OF_ACTIONS; i++)
			{

				Action a;
				a.act_type = i;
				Event event_a;
				event_a.time = t.get_current_time();
				event_a.event_type = false;
				event_a.set_action(a);
				event_a.set_index(memory.storage.read_events_unify());
				memory.storage.incr_events_unify();
				Feeling a_feel;
				a_feel.set_feelings(feelings);
				event_a.set_feeling(a_feel);
				third_thought.add_event(event_a);
				memory.add_event(event_a);

			}

			last_action.act_type = 56;
			fourth_event.time = t.get_current_time();
			fourth_event.event_type = false;
			fourth_event.set_action(last_action);
			fourth_event.set_index(memory.storage.read_events_unify());
			memory.storage.incr_events_unify();
			Feeling fourth_feel;
			fourth_feel.set_feelings(feelings);
			fourth_event.set_feeling(fourth_feel);
			fourth_thought.add_event(fourth_event);
			memory.add_event(fourth_event);
		}

		Mind()
		{

		}

		//Mind(Memory mem)
		//{

		//	std::cout << "Initializing left brain...";
		//	std::cout << std::endl;
		//	// current feelings values
		//	// start em at 0
		//	happiness = 0;
		//	sadness = 0;
		//	curiosity = 0;
		//	restlessness = 0;
		//	questioning = 0;
		//	tiredness = 0;
		//	tolerance = 1;

		//	// if you're going to modify these
		//	// make sure they arent zero and 
		//	// are less than or equal to 1
		//	// default values are just 1
		//	happy_tolerance = 1;
		//	sad_tolerance = 0.7;
		//	curious_tolerance = 0.9;
		//	rest_tolerance = 0.5;
		//	quest_tolerance = 1;
		//	tired_tolerance = 0.5;
		//	tol_tolerance = 0.8;

		//	value_diff = 0;
		//	value_count = 0;

		//	std::vector<double> feelings;
		//	feelings.push_back(happiness);
		//	feelings.push_back(sadness);
		//	feelings.push_back(restlessness);
		//	feelings.push_back(curiosity);
		//	feelings.push_back(questioning);
		//	feelings.push_back(tiredness);
		//	feelings.push_back(tolerance);
		//	// here's where we're putting the visual seeds
		//	// these images determine what the program 
		//	// likes and hates.
		//	// its the basis of the formation of attraction
		//	// and repulsion that forms the core of the
		//	// program's ability to think
		//	// i'll add more of these in the future
		//	std::string first_image_path = "happy3.jpg";
		//	std::string second_image_path = "despair.jpg";


		//	// everything here is just initializing the 
		//	// visual seed and action seed memories
		//	cv::Mat image = cv::imread(first_image_path);
		//	if (!image.data)
		//	{
		//		std::cout << ("Image 1 failed to load.\n");
		//	}
		//	else
		//	{
		//		first_experience.this_image = Image::process_image(
		//			Image::grayscale_file(
		//				first_image_path));
		//	}
		//	first_event.time = t.get_current_time();
		//	first_event.set_experience(first_experience);
		//	first_event.set_index(memory.storage.read_events_unify());
		//	memory.storage.incr_events_unify();
		//	Feeling first_feel;
		//	first_feel.set_feelings(feelings);
		//	first_event.set_feeling(first_feel);
		//	first_thought.add_event(first_event);
		//	memory.add_event(first_event);

		//	cv::Mat image2 = cv::imread(second_image_path);
		//	if (!image2.data)
		//	{
		//		std::cout << ("Image 2 failed to load.\n");
		//	}
		//	else
		//	{
		//		second_experience.this_image = Image::process_image(
		//			Image::grayscale_file(
		//				second_image_path));
		//	}
		//	second_event.time = t.get_current_time();
		//	second_event.set_experience(second_experience);
		//	second_event.set_index(memory.storage.read_events_unify());
		//	memory.storage.incr_events_unify();
		//	Feeling second_feel;
		//	second_feel.set_feelings(feelings);
		//	second_event.set_feeling(second_feel);
		//	second_thought.add_event(second_event);
		//	memory.add_event(second_event);

		//	for (int i = 0; i < NUMBER_OF_ACTIONS; i++)
		//	{

		//		Action a;
		//		a.act_type = i;
		//		Event event_a;
		//		event_a.time = t.get_current_time();
		//		event_a.event_type = false;
		//		event_a.set_action(a);
		//		event_a.set_index(memory.storage.read_events_unify());
		//		memory.storage.incr_events_unify();
		//		Feeling a_feel;
		//		a_feel.set_feelings(feelings);
		//		event_a.set_feeling(a_feel);
		//		third_thought.add_event(event_a);
		//		memory.add_event(event_a);

		//	}

		//	last_action.act_type = 56;
		//	fourth_event.time = t.get_current_time();
		//	fourth_event.event_type = false;
		//	fourth_event.set_action(last_action);
		//	fourth_event.set_index(memory.storage.read_events_unify());
		//	memory.storage.incr_events_unify();
		//	Feeling fourth_feel;
		//	fourth_feel.set_feelings(feelings);
		//	fourth_event.set_feeling(fourth_feel);
		//	fourth_thought.add_event(fourth_event);
		//	memory.add_event(fourth_event);

		//}


	};

}
