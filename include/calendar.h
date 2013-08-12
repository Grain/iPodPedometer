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
        boost::gregorian::date getSelectedDate();
        //setters
        void setYearMonth(boost::gregorian::greg_year, boost::gregorian::greg_month);
        //functions
        void draw(sf::RenderTarget*);
        void nextMonth();
        void previousMonth();
        void selectPoint(sf::Vector2i);
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
        sf::Text title;
        sf::Text days[COLUMNS * (ROWS - 1)];
        sf::RectangleShape background;
        sf::RectangleShape selectedBox;

        boost::gregorian::date currentMonth;
        boost::gregorian::date today;

        boost::gregorian::days monthLength;
        boost::gregorian::days prevLength;
        boost::gregorian::days nextLength;

        short firstDayOfWeek;   //0 = sunday, 1 = monday, etc.

        //0 for the first box on the calendar (text for name of first day of week), so 7 would be first day of month
        int firstDay;
        int selected;

        bool showSelected;
};

#endif // CALENDAR_H
