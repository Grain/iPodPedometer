#include "calendar.h"

//ctors

Calendar::Calendar()
{
    initialize();

    //first day of the current month
    setYearMonth(today.year(), today.month());
}

Calendar::Calendar(int year, int month)
{
    initialize();

    setYearMonth(year, month);
}

//dtor

Calendar::~Calendar()
{
    //dtor
}

//getters


//setters

void Calendar::setYearMonth(boost::gregorian::greg_year year, boost::gregorian::greg_month month)
{
    currentMonth = boost::gregorian::date(year, month, 1);

    //this month's number of days
    monthLength = currentMonth.end_of_month() - currentMonth + boost::gregorian::days(1);

    //previous month's number of days
    boost::gregorian::date tempMonth = currentMonth - boost::gregorian::days(1);
    tempMonth = boost::gregorian::date(tempMonth.year(), tempMonth.month(), 1);
    prevLength = tempMonth.end_of_month() - tempMonth + boost::gregorian::days(1);

    //next month's number of days
    tempMonth = currentMonth + boost::gregorian::days(35);
    tempMonth = boost::gregorian::date(tempMonth.year(), tempMonth.month(), 1);
    nextLength = tempMonth.end_of_month() - tempMonth + boost::gregorian::days(1);

    //get a boost::gregorian::date that is the day of the week as the firstdayofweek variable
    boost::gregorian::date tempDay = today;
    while(tempDay.day_of_week() != firstDayOfWeek)
    {
        tempDay += boost::gregorian::days(1);
    }

    //day of week text and day of month numbers

    int currentCount = 0;
    int nextCount = 0;

    for (int i = 0; i < COLUMNS * (ROWS - 1); ++i)
    {
        days[i].setPosition(sf::Vector2f(position.x + (i % 7) * WIDTH + WIDTH / 2, position.y + (i / 7 + 1) * HEIGHT + HEIGHT / 2));
        days[i].setCharacterSize(12);
        days[i].setStyle(sf::Text::Regular);
        if (i < 7)
        {
            std::stringstream temp;
            temp << (tempDay + boost::gregorian::days(i)).day_of_week();

            days[i].setColor(sf::Color::Black);
            days[i].setString(temp.str());
        }

        else
        {
            if (currentCount == 0)
            {
                if (calculatePrev(i) == -1)
                {
                    currentCount = 1;   //done displaying last month
                }
                else    //display last month
                {
                    days[i].setColor(sf::Color::Green);
                    char temp[20];
                    sprintf(temp, "%d", calculatePrev(i));
                    days[i].setString(temp);
                }
            }

            if (currentCount != 0)
            {
                if (nextCount == 0) //display this month
                {
                    days[i].setColor(sf::Color::Black);
                    char temp[20];
                    sprintf(temp, "%d", currentCount);
                    days[i].setString(temp);

                    if (++currentCount > monthLength.days())
                    {
                        nextCount = 1;  //done displaying this month
                    }
                }
                else    //display next month
                {
                    days[i].setColor(sf::Color::Red);
                    char temp[20];
                    sprintf(temp, "%d", nextCount);
                    days[i].setString(temp);

                    nextCount++;
                }
            }


        }

        days[i].setOrigin(sf::Vector2f(days[i].getGlobalBounds().width / 2, days[i].getGlobalBounds().height / 2));
    }
}

//functions

void Calendar::nextMonth()
{
    if (currentMonth.month() == 12)
    {
        setYearMonth(currentMonth.year() + 1, 1);
    }
    else
    {
        setYearMonth(currentMonth.year(), currentMonth.month() + 1);
    }
}

void Calendar::previousMonth()
{
    if (currentMonth.month() == 1)
    {
        setYearMonth(currentMonth.year() - 1, 12);
    }
    else
    {
        setYearMonth(currentMonth.year(), currentMonth.month() - 1);
    }
}

int Calendar::calculatePrev(int i)
{
    short dayOfWeek = (i + firstDayOfWeek) % 7;
    short firstDay = currentMonth.day_of_week();

    if (firstDay < dayOfWeek)     //will display last month
    {
        firstDay += 7;
    }
    else if (i == 7)
    {
        if (firstDay == dayOfWeek)
        {
            firstDay += 7;
        }
    }
    else if (firstDay == dayOfWeek)     //start displaying this month
    {
        return -1;
    }

    return prevLength.days() - (firstDay - dayOfWeek) + 1;
}

void Calendar::initialize()
{
    today = boost::gregorian::date(boost::gregorian::day_clock::local_day());

    position = sf::Vector2f(100, 100);
    firstDayOfWeek = 0;

    font.loadFromFile("arial.ttf");

    for (int i = 0; i < COLUMNS * (ROWS - 1); ++i)
    {
        days[i].setFont(font);
    }
}

void Calendar::draw(sf::RenderTarget * target)
{
    sf::RectangleShape line;
    line.setFillColor(sf::Color::Black);

    for(int a = 0; a <= COLUMNS; ++a)
    {
        if (a == 0 || a == COLUMNS)
        {
            line.setSize(sf::Vector2f(1, (float)(ROWS * HEIGHT)));
            line.setPosition(sf::Vector2f(position.x + a * WIDTH, position.y));
        }
        else
        {
            line.setSize(sf::Vector2f(1, (float)((ROWS - 1) * HEIGHT)));
            line.setPosition(sf::Vector2f(position.x + a * WIDTH, position.y + HEIGHT));
        }

        target->draw(line);
    }

    for(int a = 0; a <= ROWS; ++a)
    {
        line.setSize(sf::Vector2f{(float)(COLUMNS * WIDTH), 1});
        line.setPosition(sf::Vector2f(position.x, position.y + a * HEIGHT));
        target->draw(line);
    }

    for (int i = 0; i < COLUMNS * (ROWS - 1); ++i)
    {
        target->draw(days[i]);
    }

    std::cout << currentMonth << std::endl;
}
