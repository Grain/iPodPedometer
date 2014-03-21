#include <SFML/Graphics.hpp>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
//#include <algorithm>
#include "calendar.h"
#include <boost/filesystem.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

std::vector<std::pair<boost::gregorian::date, int>> records;

void addRecord(boost::gregorian::date day, int steps)
{
    std::cout << "added" << std::endl;
    for(unsigned int i = 0; i < records.size(); ++i)
    {
        if (records[i].first == day)
        {
            records[i].second += steps;
            return;
        }
    }

    records.push_back(std::make_pair(day, steps));
}

int main(int argc, char* argv[])
{
//    boost::filesystem::path path("G:/iPod_Control/Device/Trainer/Workouts/Empeds/pedometer");
//    boost::filesystem::path path("E:/pedometer");
    boost::filesystem::path path;

    if (argc > 2 && std::string(argv[1]) == "-i")
    {
        path = boost::filesystem::path(std::string(argv[2]) + "/iPod_Control/Device/Trainer/Workouts/Empeds/pedometer");
    }
    else if (argc > 2 && std::string(argv[1]) == "-d")
    {
        path = boost::filesystem::path(std::string(argv[2]));
    }
    else
    {
        std::cout << "Use the -i [DIR] argument to supply the location of the iPod." << std::endl;
        std::cout << "Use the -d [DIR] to supply the location of the pedometer directory." << std::endl;
        std::cout << "Press the enter key to exit.";
        std::cin.get();
        exit(0);
    }

    Calendar calendar;

    boost::filesystem::recursive_directory_iterator pathIterator(path);

    while (pathIterator != boost::filesystem::recursive_directory_iterator())
    {
        if (boost::filesystem::is_regular_file(pathIterator->path()))
        {
            if (pathIterator->path().filename() != "lastWorkout.xml" && pathIterator->path().filename() != "settings.plist")
            {
                //DO IT
                const std::string DATE_BEGIN = "<time>";
                const std::string DATE_END = "T";
                const std::string STEPS_BEGIN = "<walkEnd>";
                const std::string STEPS_END = "</walkEnd>";

                //std::cout << pathIterator->path().generic_string() << std::endl;

                std::ifstream file(pathIterator->path().generic_string());
                std::string line;

                if (file.is_open())
                {
                    boost::gregorian::date day;
                    int steps = -1;

                    while (file.good())
                    {
                        std::getline(file, line);

                        //date
                        std::size_t first = line.find(DATE_BEGIN);
                        std::size_t last = line.find(DATE_END);
                        if (first != std::string::npos && last != std::string::npos)
                        {
                            day = boost::gregorian::date(boost::gregorian::from_string(line.substr(first + DATE_BEGIN.length(), last - first - DATE_BEGIN.length())));
                        }

                        //steps
                        first = line.find(STEPS_BEGIN);
                        last = line.find(STEPS_END);
                        if (first != std::string::npos && last != std::string::npos)
                        {
                            steps = atoi(line.substr(first + STEPS_BEGIN.length(), last - first - DATE_BEGIN.length()).c_str());
                        }

                        if (day != boost::gregorian::date() && steps != -1)
                        {
                            addRecord(day, steps);
                            break;
                        }
                    }
                    file.close();
                }
            }
        }
        pathIterator++;
    }

//    std::cout << boost::filesystem::directory_iterator(path)->path().filename() << std::endl;
//    copy(boost::filesystem::directory_iterator(path), boost::filesystem::directory_iterator(), std::ostream_iterator<boost::filesystem::directory_entry>(std::cout, "\n"));

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Test", sf::Style::Close);
    window.setFramerateLimit(30);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::KeyPressed: //for debugging
                {
                    if (event.key.code == sf::Keyboard::Key::Left)
                    {
                        calendar.previousMonth();
                    }
                    if (event.key.code == sf::Keyboard::Key::Right)
                    {
                        calendar.nextMonth();
                    }
                    if (event.key.code == sf::Keyboard::Key::Up)
                    {
                        std::cout << calendar.getSelectedDate() << std::endl;
                    }
                    if (event.key.code == sf::Keyboard::Key::Down)
                    {
//                        for (unsigned int i = 0; i < records.size(); ++i)
//                        {
//                            std::cout << records[i].first << " " << records[i].second << std::endl;
//                        }
                        bool found = false;
                        for (unsigned int i = 0; i < records.size(); ++i)
                        {
                            if (calendar.getSelectedDate() == records[i].first)
                            {
                                std::cout << records[i].second << std::endl;
                                found = true;
                                break;
                            }
//                            std::cout << records[i].first << " " << records[i].second << std::endl;
                        }

                        if (found == false)
                        {
                            std::cout << "nothing that day" << std::endl;
                        }
                    }
                }
                default:
                {
                    break;
                }
            }
        }
        calendar.selectPoint(sf::Mouse::getPosition(window));

        window.clear(sf::Color::White);

        calendar.draw(&window);

        window.display();
    }

    return 0;
}
