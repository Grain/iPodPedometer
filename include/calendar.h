#ifndef CALENDAR_H
#define CALENDAR_H

#include <SFML/Graphics.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <cstdio>

class Calendar
{
    public:
        //ctor/dtor
        Calendar();
        Calendar(int, int);
        virtual ~Calendar();
        //getters

        //setters
        void setYearMonth(boost::gregorian::greg_year, boost::gregorian::greg_month);
        //functions
        void draw(sf::RenderTarget*);
        void nextMonth();
        void previousMonth();
    protected:
    private:
        //functions
        void initialize();
        int calculatePrev(int);
        //consts
        const static short COLUMNS = 7;
        const static short ROWS = 8;   //6 + day of week names + title row
        const static short WIDTH = 40;
        const static short HEIGHT = 30;
        //vars
        std::string daysOfWeek[7];

        sf::Vector2f position;
        sf::Font font;
        sf::Text days[COLUMNS * (ROWS - 1)];

        boost::gregorian::date currentMonth;
        boost::gregorian::date today;

        boost::gregorian::days monthLength;
        boost::gregorian::days prevLength;
        boost::gregorian::days nextLength;

        short firstDayOfWeek;   //0 = sunday, 1 = monday, etc.
};

#endif // CALENDAR_H
