#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <cctype>
#include <iomanip>

using namespace std;

// Function to trim leading quotes
string trimLeadingQuotes(const string &str)
{
    string result = str;
    if (!result.empty() && result.front() == '"')
    {
        result.erase(result.begin()); // Remove the leading double quote
    }
    return result;
}

// Function to trim trailing quotes
string trimTrailingQuotes(const string &str)
{
    string result = str;
    if (!result.empty() && result.back() == '"')
    {
        result.pop_back(); // Remove the trailing double quote
    }
    return result;
}

// Function to trim both leading and trailing quotes
string trimQuotes(const string &str)
{
    string result = trimLeadingQuotes(str);
    result = trimTrailingQuotes(result);
    return result;
}

string getCurrentDate() //to get the date today
{
    time_t t = time(0);
    struct tm *now = localtime(&t);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", now);
    return buf;
}

bool isLeapYear(int year) //leap year checker
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(int day, int month, int year) //valid date 
{
    if (year < 1 || month < 1 || month > 12 || day < 1)
    {
        return false;
    }
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Adjust for leap year
    if (month == 2 && isLeapYear(year))
    {
        daysInMonth[1] = 29;
    }

    return day <= daysInMonth[month - 1];
}

bool isValidDateFormat(const string &date) //correct format
{
    if (date.length() != 10 || date[4] != '-' || date[7] != '-')
    {
        return false;
    }
    int day, month, year;
    char dash1, dash2;
    stringstream ss(date);

    if (!(ss >> year >> dash1 >> month >> dash2 >> day))
    {
        return false;
    }
    if (dash1 != '-' || dash2 != '-')
    {
        return false;
    }
    return isValidDate(day, month, year);
}

class Reservation //to help in maintaining reservations
{
public:
    string congregationName;
    string startDate;
    string endDate;

    Reservation(const string &congregationName, const string &startDate, const string &endDate)
        : congregationName(congregationName), startDate(startDate), endDate(endDate) {}
};

class Event //to help in events declaration
{
public:
    string name;
    string date;
    string start_time;
    string end_time;

    Event(const string &name, const string &date, const string &start_time, const string &end_time)
        : name(name), date(date), start_time(start_time), end_time(end_time) {}
};

class Venue //venue declared
{
public:
    string name;
    string location;
    string city;
    string state;
    string postal_code;
    string country;
    int capacity;

    Venue(const string &name, const string &location, const string &address, const string &state, const string &postal_code, const string &country, int capacity)
        : name(name), location(location), city(address), state(state), postal_code(postal_code), country(country), capacity(capacity)
    {
        // cout<<capacity<<endl;
    }

    vector<Event> event_tracker; //events per venue
    vector<Reservation> reservation_tracker; //reservations per venue
};

class Congregation //congragations similar to venue in second assignment
{
public:
    string name;
    string type;
    string start_date;
    string end_date;

    Congregation(const string &name, const string &type, const string &startDate, const string &endDate)
        : name(name), type(type), start_date(startDate), end_date(endDate) {}
};

class Congregation_helper //for all congragation functions
{
private:
    vector<Venue> venues; //venue list

public:
    vector<Congregation> cong_list; //cong_list

    void addCongregation(const string &name, const string &type, const string &start_date, const string &end_date) //addition
    {
        int flag = 0; //to check for erris
        string current_date = getCurrentDate();

        // Check if type is valid
        if (type != "Concert" && type != "Games" && type != "Convention" && type != "Conference")
        {
            flag++;
        }

        // Check for non-empty name and dates
        if (name.empty() || start_date.empty() || end_date.empty())
        {
            flag++;
        }

        // Check for valid date formats and date range
        if (!isValidDateFormat(start_date) || !isValidDateFormat(end_date))
        {
            flag++;
        }
        if (start_date > end_date)
        {
            flag++;
        }

        // Check if the event's dates are in the past compared to today
        if (end_date < current_date)
        {
            flag++;
        }

        // Check for existing congregation with the same name
        for (const auto &cong : cong_list)
        {
            if (cong.name == name)
            {
                flag++;
                break;
            }
        }

        // If any flag non zero then error
        if (flag)
        {
            cout << "-1\nError\n";
        }
        else
        {
            Congregation cong(name, type, start_date, end_date);
            cong_list.push_back(cong); //successful
            cout << "0\n";
        }
    }

    void deleteCongregation(const string &name) //find the congregation and delete if found
    {
        bool found = false; //to check for error cases
        for (auto it = cong_list.begin(); it != cong_list.end();)
        {
            if (it->name == name)
            {
                it = cong_list.erase(it);
                found = true;
            }
            else
            {
                ++it;
            }
        }

        if (found)
        {
            cout << "0\n";

            // Remove reservations for venue
            for (auto &v : venues)
            {
                for (auto it = v.reservation_tracker.begin(); it != v.reservation_tracker.end();)
                {
                    if (it->congregationName == name)
                    {
                        // Remove related events
                        for (auto i = v.event_tracker.begin(); i != v.event_tracker.end();)
                        {
                            if (i->start_time == it->startDate && i->end_time == it->endDate)
                            {
                                i = v.event_tracker.erase(i);
                            }
                            else
                            {
                                ++i;
                            }
                        }
                        it = v.reservation_tracker.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }
        else
        {
            cout << "-1\nError\n";
        }
    }

    void showCongregations() //show congregations
    {
        if (cong_list.size() == 0)
        {
            cout << "0\n";
            return;
        }
        cout << cong_list.size() << endl;
        for (auto &c : cong_list)
        {
            cout << c.name << " " << c.type << " " << c.start_date << " " << c.end_date << "\n";
        }
    }
};

// venue helper now

class Venue_Helper
{
public:
    vector<Venue> venues;

    void addVenue(const string &name, const string &location, const string &city, const string &state, const string &postalCode, const string &country, int capacity) //addvenue
    {
        // Trim leading and trailing quotes
        string trimmedCity = trimTrailingQuotes(trimLeadingQuotes(city));
        string trimmedState = trimTrailingQuotes(trimLeadingQuotes(state));
        string trimmedPostalCode = trimTrailingQuotes(trimLeadingQuotes(postalCode));
        string trimmedCountry = trimTrailingQuotes(trimLeadingQuotes(country));

        // Error checking
        int flag = 0; 
        if (name.empty() || trimmedCountry.empty() || capacity <= 0)
        {
            flag++;
        }
        for (const auto &v : venues)
        {
            if (v.name == name && v.country == trimmedCountry) //venue already there
            {
                flag++;
                break;
            }
        }
        if (flag)
        {
            cout << "-1\nError\n";
            return;
        }
        // cout<<capacity<<endl;
        // Add the new venue
        venues.push_back(Venue(name, location, trimmedCity, trimmedState, trimmedPostalCode, trimmedCountry, capacity));
        cout << "0\n"; //successful
    }

    void deleteVenue(const string &name, const string &country, vector<Congregation> &cong_list) //delete from congregations
    {
        Venue *v = nullptr;
        int idx = -1;

        // Find the venue
        for (int i = 0; i < venues.size(); ++i)
        {
            if (venues[i].name == name && venues[i].country == country) //venue found
            {
                v = &venues[i];
                idx = i;
                break;
            }
        }

        if (v == nullptr)
        {
            cout << "-1\nError\n"; // Venue not found
            return;
        }

        // Check if any congregation is associated with this venue
        for (const auto &cong : cong_list)
        {
            for (const auto &reservation : v->reservation_tracker)
            {
                if (reservation.congregationName == cong.name)
                {
                    cout << "-1\nError\n"; // Venue still associated with a congregation
                    return;
                }
            }
        }

        // If no associated congregations, delete the venue
        venues.erase(venues.begin() + idx);
        cout << "0\n"; //successful
    }

    int showVenues(const string &location) //showVenue
    {
        // Trim leading and trailing quotes from the location string
        string trimmedLocation = trimQuotes(location);

        string city, state, postalCode, country;

        // Parse the location string
        stringstream ss(trimmedLocation);
        getline(ss, city, ':');
        getline(ss, state, ':');
        getline(ss, postalCode, ':');
        getline(ss, country, ':');

        vector<Venue> display;

        //all the cases where thereis a valid location format
        if (!country.empty() && city.empty() && state.empty() && postalCode.empty())
        {
            for (auto &v : venues)
            {
                if (v.country == country)
                {
                    display.push_back(v);
                }
            }
        }
        else if (!country.empty() && !city.empty() && !state.empty() && postalCode.empty())
        {
            for (auto &v : venues)
            {
                if (v.country == country && v.city == city && v.state == state)
                {
                    display.push_back(v);
                }
            }
        }
        else if (!country.empty() && !city.empty() && !state.empty() && !postalCode.empty())
        {
            for (auto &v : venues)
            {
                if (v.country == country && v.city == city && v.state == state && v.postal_code == postalCode)
                {
                    display.push_back(v);
                }
            }
        }
        else if (!country.empty() && city.empty() && state.empty() && !postalCode.empty())
        {
            for (auto &v : venues)
            {
                if (v.country == country && v.postal_code == postalCode)
                {
                    display.push_back(v);
                }
            }
        }
        else //invalid format
        {
            cout << "-1\nError\n";
            return -1;
        }

        if (display.empty()) //empty
        {
            cout << "0\n";
        }
        else
        {
            cout << display.size() << "\n";
            for (auto &v : display)
            {
                // Clean up the location string and remove space and traling quote
                if (v.location.front() == ' ' && v.location[1] == '"')
                {
                    v.location.erase(0, 2);
                }

                cout << v.name << " " << v.location << ":" << v.city << ":"
                     << v.state << ":" << v.postal_code << ":" << v.country
                     << " " << v.capacity << "\n";
            }
        }

        return 0;
    }

    void reserveVenue(const string &venue_name, const string &venue_country, const string &congregation_name, const vector<Congregation> &cong_list) //reserve Venue
    {
        const Congregation *cong = nullptr; //find cong
        for (auto &congs : cong_list)
        {
            if (congs.name == congregation_name)
            {
                cong = &congs;
                break;
            }
        }

        if (!cong) //cong not found
        {
            cout << "-1\nError\n";
            return;
        }

        for (int i = 0; i < venues.size(); ++i) 
        {
            if (venues[i].name == venue_name && venues[i].country == venue_country)
            {
                // Check for overlapping reservations
                for (const auto &res : venues[i].reservation_tracker)
                {
                    if (!(cong->end_date < res.startDate || cong->start_date > res.endDate))
                    {
                        cout << "-1\nError\n"; //: Date overlap with existing reservation
                        return;
                    }
                }

                // No overlap, so we can reserve the venue
                venues[i].reservation_tracker.push_back(Reservation(cong->name, cong->start_date, cong->end_date));
                cout << "0\n"; // Success
                return;
            }
        }
        cout << "-1\nError\n"; // venue
    }

    void freeVenue(const string &venue_name, const string &venue_country, const string &congregation_name, const vector<Congregation> &cong_list) //freeVenue
    {
        const Congregation *cong = nullptr;
        for (auto &congs : cong_list)
        {
            if (congs.name == congregation_name)
            {
                cong = &congs;
            }
        }

        if (!cong)
        {
            cout << "-1\nError\n";
            return;
        }

        Venue *ven_temp = nullptr;
        for (auto &venue : venues)
        {
            if (venue.name == venue_name && venue.country == venue_country)
            {
                ven_temp = &venue;
                break;
            }
        }

        if (!ven_temp)
        {
            cout << "-1\nError\n";
        }
        for (auto it = ven_temp->reservation_tracker.begin(); it != ven_temp->reservation_tracker.end(); ++it)
        {
            if (it->congregationName == congregation_name)
            {
                ven_temp->reservation_tracker.erase(it);
                cout << "0\n"; // Success
                return;
            }
        }
        cout << "-1\nError\n";
    }

    void showReserved(const string &congregation_name, const vector<Congregation> &cong_list)
    {
        const Congregation *cong = nullptr;
        for (const auto &congs : cong_list)
        {
            if (congs.name == congregation_name)
            {
                cong = &congs;
                break; // Once found, no need to continue the loop
            }
        }

        if (!cong)
        {
            cout << "-1\nError\n";
            return;
        }

        int count = 0;
        // First pass: Count the number of reserved venues for this congregation
        for (const auto &venue : venues)
        {
            for (const auto &res : venue.reservation_tracker)
            {
                if (res.congregationName == congregation_name)
                {
                    count++;
                    break; // No need to continue if a reservation is found for this venue
                }
            }
        }

        // Print the number of reserved venues
        cout << count << "\n";

        // Second pass: Print the details of each reserved venue
        if (count > 0)
        {
            for (auto &venue : venues)
            {
                for (const auto &res : venue.reservation_tracker)
                {
                    if (res.congregationName == congregation_name)
                    {
                        if (venue.location.front() == ' ' && venue.location[1] == '"')
                        {
                            venue.location.erase(0, 2);
                        }
                        cout << venue.name << " " << venue.location << ":" << venue.city << ":" << venue.state << ":" << venue.postal_code << ":" << venue.country << " " << venue.capacity << "\n";
                    }
                }
            }
        }
    }
};

class Event_helper
{
public:
    vector<Event> event_tracker;

    bool isValidTime(const string &time)
    {
        // Time should be exactly 5 characters long in the format "HH:MM"
        if (time.length() != 5 || time[2] != ':')
        {
            return false;
        }

        // Check if the hour and minute parts are digits
        if (!isdigit(time[0]) || !isdigit(time[1]) || !isdigit(time[3]) || !isdigit(time[4]))
        {
            return false;
        }

        // Extract the hour and minute parts
        int hour = stoi(time.substr(0, 2));
        int minute = stoi(time.substr(3, 2));

        // Validate hour and minute ranges
        if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
        {
            return false;
        }

        return true;
    }

    int getMinutes(const string &time)
    {
        int hour = stoi(time.substr(0, 2));
        int minute = stoi(time.substr(3, 2));
        return hour * 60 + minute;
    }

    int event_is_valid_timing(const string &start_time, const string &end_time)
    {
        int flag = 0;
        if (!(isValidTime(start_time) && (end_time == "00:00" || isValidTime(end_time))))
        {
            return flag;
        }
        int start;
        int end;

        start = getMinutes(start_time);
        if (end_time == "00:00")
        {
            end = getMinutes("23:59") + 1;
        }
        else
        {
            end = getMinutes(end_time);
        }

        int event_time = end - start;
        if (event_time <= 0 && end_time != "00:00")
        {
            return flag;
        }

        if (event_time < 30 || event_time > 1440)
        {
            return flag;
        }

        flag = 1;
        return flag;
    }

    string getPreviousDate(const string &date) // helper function to find previous date from a given date
    {
        tm timeStruct = {};
        istringstream ss(date);
        ss >> get_time(&timeStruct, "%Y-%m-%d");

        timeStruct.tm_mday -= 1;

        mktime(&timeStruct);

        ostringstream oss;
        oss << put_time(&timeStruct, "%Y-%m-%d");
        return oss.str();
    }

    bool checkOverlap(const string &fromTime, const string &toTime, const vector<Event> &events, const string &date, const vector<Event> &allEvents) // helper function to check if dates of an event are overlapping
    {
        int fromMinutes;
        int toMinutes;

        if (fromTime == "00:00")
        {
            fromMinutes = 0;
        }
        else
        {
            fromMinutes = getMinutes(fromTime);
        }

        if (toTime == "00:00")
        {
            toMinutes = 1440;
        }
        else
        {
            toMinutes = getMinutes(toTime);
        }

        for (const auto &event : events)
        {
            int existingFromMinutes;
            int existingToMinutes;

            if (event.start_time == "00:00")
            {
                existingFromMinutes = 0;
            }
            else
            {
                existingFromMinutes = getMinutes(event.start_time);
            }

            if (event.end_time == "00:00")
            {
                existingToMinutes = 1440;
            }
            else
            {
                existingToMinutes = getMinutes(event.end_time);
            }

            if (!(toMinutes <= existingFromMinutes || fromMinutes >= existingToMinutes))
            {
                return true;
            }

            if (abs(fromMinutes - existingToMinutes) < 30 || abs(toMinutes - existingFromMinutes) < 30)
            {
                return true;
            }
        }

        // checking if it overlaps with one from the previous date
        string previousDate = getPreviousDate(date);
        int lastEventEndTime = -1;

        for (const auto &e : allEvents)
        {
            if (e.date == previousDate)
            {
                int eventEndMinutes;
                if (e.end_time == "00:00")
                {
                    eventEndMinutes = 1440;
                }
                else
                {
                    eventEndMinutes = getMinutes(e.end_time);
                }
                lastEventEndTime = max(lastEventEndTime, eventEndMinutes);
            }
        }

        if (lastEventEndTime != -1)
        {
            if (fromMinutes == 0 && (1440 - lastEventEndTime) < 30)
            {
                return true;
            }
            else if (fromMinutes > 0 && abs(fromMinutes - lastEventEndTime) < 30)
            {
                return true;
            }
        }

        return false;
    }

    void addEvent(const string &cong_name, const string &venueName, const string &country, const string &date, const string &fromTime, const string &toTime, const string &eventName, vector<Congregation> &cong_list, vector<Venue> &venues) // function to add a new event
    {

        Venue *v = nullptr;
        for (int i = 0; i < venues.size(); ++i)
        {
            if (venues[i].name == venueName && venues[i].country == country)
            {
                v = &venues[i];
                break;
            }
        }

        if (v == nullptr)
        {
            cout << "-1\nError \n"; // Venue not found
            return;
        }

        // congregaton finder

        Congregation *c = nullptr;

        for (int i = 0; i < cong_list.size(); ++i)
        {
            if (cong_list[i].name == cong_name)
            {
                c = &cong_list[i];
                break;
            }
        }

        if (c == nullptr)
        {
            cout << "-1\nError\n"; // Venue not found
            return;
        }

        // reservation finder

        Reservation *reservation = nullptr;
        for (int i = 0; i < v->reservation_tracker.size(); ++i)
        {
            if (v->reservation_tracker[i].congregationName == cong_name)
            {
                reservation = &v->reservation_tracker[i];
                break; // Exit loop once the reservation is found
            }
        }

        if (reservation == nullptr)
        {
            cout << "-1\nError\n"; // Reservation not found
            return;
        }

        if (!event_is_valid_timing(fromTime, toTime))
        {
            cout << "-1\nError\n";
            return;
        }

        vector<Event> existingEvents;

        for (const auto &e : v->event_tracker) // checking for overlap
        {
            if (e.date == date)
            {
                existingEvents.push_back(e);
            }
        }

        if (checkOverlap(fromTime, toTime, existingEvents, date, v->event_tracker))
        {
            cout << "-1\nError\n";
            return;
        }

        Event newEvent(eventName, date, fromTime, toTime);
        v->event_tracker.push_back(newEvent);

        cout << "0\n";
        return;
    }

    void deleteEvent(const string &cong_name, const string &venueName, const string &country, const string &date, const string &fromTime, const string &eventName, vector<Congregation> &cong_list, vector<Venue> &venues)
    {
        Venue *v = nullptr;
        for (int i = 0; i < venues.size(); ++i)
        {
            if (venues[i].name == venueName && venues[i].country == country)
            {
                v = &venues[i];
                break;
            }
        }

        if (v == nullptr)
        {
            cout << "-1\nError\n"; // Venue not found
            return;
        }

        Reservation *reservation = nullptr;
        for (int i = 0; i < v->reservation_tracker.size(); ++i)
        {
            if (v->reservation_tracker[i].congregationName == cong_name)
            {
                reservation = &v->reservation_tracker[i];
                break; // Exit loop once the reservation is found
            }
        }

        if (reservation == nullptr)
        {
            cout << "-1\nError \n"; // Reservation not found
            return;
        }

        Event *e = nullptr;
        int eventIndex = -1;
        for (int i = 0; i < v->event_tracker.size(); ++i)
        {
            if (v->event_tracker[i].name == eventName && v->event_tracker[i].date == date && v->event_tracker[i].start_time == fromTime)
            {
                e = &v->event_tracker[i];
                eventIndex = i;
                break;
            }
        }

        if (e == nullptr)
        {
            cout << "-1\nError \n"; // Event not found
            return;
        }

        // Remove the event from the event_tracker list
        v->event_tracker.erase(v->event_tracker.begin() + eventIndex);
        cout << "0\n";
    }

    void bubbleSortEventsByTime(vector<Event> &events)
    {
        int n = events.size();
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (events[j].start_time > events[j + 1].start_time)
                {
                    // Swap the events if they are in the wrong order
                    swap(events[j], events[j + 1]);
                }
            }
        }
    }

    void showEvents(const string &venue_name, const string &country, const string &date, vector<Congregation> &cong_list, vector<Venue> &venues)
    {
        Venue *v = nullptr;
        for (int i = 0; i < venues.size(); ++i)
        {
            if (venues[i].name == venue_name && venues[i].country == country)
            {
                v = &venues[i];
                break;
            }
        }

        if (v == nullptr)
        {
            cout << "-1\nError\n"; // Venue not found
            return;
        }

        if (!isValidDateFormat(date))
        {
            cout << "-1\nError \n"; // Invalid date
        }

        vector<Event> valid;
        for (int i = 0; i < v->event_tracker.size(); ++i)
        {
            if (v->event_tracker[i].date == date)
            {
                valid.push_back(v->event_tracker[i]);
            }
        }

        if (!valid.size())
        {
            cout << "0\n"; // No events found
            return;
        }
        bubbleSortEventsByTime(valid);
        cout << valid.size() << "\n";
        for (int i = 0; i < valid.size(); ++i)
        {
            cout << valid[i].name << " " << valid[i].start_time << " " << valid[i].end_time << endl;
        }
    }
    vector<string> getDates(const string &startDate, const string &endDate)
    {
        vector<string> dates;
        tm tm_start = {};
        tm tm_end = {};

        istringstream ss_start(startDate);
        ss_start >> get_time(&tm_start, "%Y-%m-%d");

        istringstream ss_end(endDate);
        ss_end >> get_time(&tm_end, "%Y-%m-%d");

        // Convert tm structures to time_t
        time_t start = mktime(&tm_start);
        time_t end = mktime(&tm_end);

        // Check if the start date is after the end date
        if (start > end)
        {
            // cout << "Start date is after the end date." << endl;
            return dates; // Return empty vector
        }

        while (start <= end)
        {
            // Format the current date back to string
            ostringstream oss;
            oss << put_time(&tm_start, "%Y-%m-%d");
            dates.push_back(oss.str());

            // Increment the date by one day
            tm_start.tm_mday++;
            start = mktime(&tm_start);
        }

        return dates;
    }

    void showCalendar(const string &cong_name, const string &venue_name, const string &country, vector<Congregation> &cong_lis, vector<Venue> &venues)
    {
        Venue *v = nullptr;
        for (int i = 0; i < venues.size(); ++i)
        {
            if (venues[i].name == venue_name && venues[i].country == country)
            {
                v = &venues[i];
                break;
            }
        }

        if (v == nullptr)
        {
            cout << "-1\nError\n"; // Venue not found
            return;
        }

        Reservation *reservation = nullptr;
        for (int i = 0; i < v->reservation_tracker.size(); ++i)
        {
            if (v->reservation_tracker[i].congregationName == cong_name)
            {
                reservation = &v->reservation_tracker[i];
                break; // Exit loop once the reservation is found
            }
        }

        if (reservation == nullptr)
        {
            cout << "-1\nError\n"; // Reservation not found
            return;
        }

        vector<string> allDates = getDates(reservation->startDate, reservation->endDate);
        vector<Event> eventsInRange;

        // Collect events within the date range
        for (const auto &event : v->event_tracker)
        {
            if (event.date >= reservation->startDate && event.date <= reservation->endDate)
            {
                eventsInRange.push_back(event);
            }
        }

        if (eventsInRange.empty())
        {
            cout << "0\n"; // No events found
            return;
        }

        // Sort events by date and then by start time
        for (int i = 0; i < eventsInRange.size(); ++i)
        {
            for (int j = 0; j < eventsInRange.size() - 1; ++j)
            {
                if (eventsInRange[j].date > eventsInRange[j + 1].date ||
                    (eventsInRange[j].date == eventsInRange[j + 1].date && eventsInRange[j].start_time > eventsInRange[j + 1].start_time))
                {
                    swap(eventsInRange[j], eventsInRange[j + 1]);
                }
            }
        }

        cout << eventsInRange.size() << "\n"; // Total number of events

        // Print events for each date in the range
        for (const auto &date : allDates)
        {
            int eventCount = 0;
            for (const auto &event : eventsInRange)
            {
                if (event.date == date)
                {
                    if (eventCount == 0)
                    {
                        cout << date << " " << eventCount + 1 << "\n"; // Print date and count as soon as the first event is found
                    }
                    cout << event.name << " " << event.start_time << " " << event.end_time << "\n";
                    eventCount++;
                }
            }
            if (eventCount == 0)
            {
                cout << date << " 0\n"; // Print the date and 0 only once if no events are found
            }
        }
    }
};

int main()
{
    Congregation_helper cong_helper;
    Venue_Helper venue_helper;
    Event_helper event_helper;
    string command;

    while (getline(cin, command))
    {
        stringstream ss(command);
        string action;
        ss >> action;

        if (action == "addCongregation")
        {
            string name, type, start_date, end_date;

            // Extract the name, which is in quotes
            getline(ss, name, '\"');
            getline(ss, name, '\"');

            // Extract the type
            ss >> ws; // Skip any whitespace
            getline(ss, type, '\"');
            getline(ss, type, '\"');

            // Extract the start date
            ss >> ws; // Skip any whitespace
            getline(ss, start_date, '\"');
            getline(ss, start_date, '\"');

            // Extract the end date
            ss >> ws; // Skip any whitespace
            getline(ss, end_date, '\"');
            getline(ss, end_date, '\"');

            // Call addCongregation with parsed values
            cong_helper.addCongregation(name, type, start_date, end_date);
        }
        else if (action == "deleteCongregation")
        {
            string name;

            // Extract the name
            getline(ss, name, '\"');
            getline(ss, name, '\"');
            cong_helper.deleteCongregation(name);
        }
        else if (action == "showCongregations")
        {
            cong_helper.showCongregations();
        }
        else if (action == "addVenue")
        {
            string name, location, city, state, postalCode, country, temp;
            int capacity;

            // Extract the name
            getline(ss, name, '\"');
            getline(ss, name, '\"');

            // Extract location, city, state, postal code, country
            getline(ss, location, ':');
            getline(ss, city, ':');
            getline(ss, state, ':');
            getline(ss, postalCode, ':');
            getline(ss, country, ' '); // Extract until space
            getline(ss, temp);         // Extract until space

            // Trim any leading/trailing spaces from the country string
            country = trimLeadingQuotes(trimTrailingQuotes(country));

            // Now try to extract capacity, ensuring no leading whitespace remains
            // cout<<temp<<endl;
            capacity = stoi(temp);
            // cout<<capacity<<endl;

            // Call addVenue with parsed values
            venue_helper.addVenue(name, location, city, state, postalCode, country, capacity);
        }
        else if (action == "deleteVenue")
        {
            string name, country;

            // Extract the name
            getline(ss, name, '\"');
            getline(ss, name, '\"');

            // Extract the country
            ss >> ws; // Skip any whitespace
            getline(ss, country, '\"');
            getline(ss, country, '\"');

            venue_helper.deleteVenue(name, country,cong_helper.cong_list);
        }
        else if (action == "showVenues")
        {
            string location;
            getline(ss, location, '\"');
            getline(ss, location, '\"');

            venue_helper.showVenues(location);
        }
        else if (action == "reserveVenue")
        {
            string venue_name, venue_country, congregation_name;

            // Extract the venue name
            if (getline(ss, venue_name, '\"'))
            {
                getline(ss, venue_name, '\"'); // Read up to the second quote
            }

            // Extract the venue country
            if (getline(ss >> ws, venue_country, '\"'))
            {
                getline(ss, venue_country, '\"'); // Read up to the second quote
            }

            // Extract the congregation name
            if (getline(ss >> ws, congregation_name, '\"'))
            {
                getline(ss, congregation_name, '\"'); // Read up to the second quote
            }

            // Call the reserveVenue function with extracted values
            venue_helper.reserveVenue(venue_name, venue_country, congregation_name, cong_helper.cong_list);
        }

        else if (action == "freeVenue")
        {
            string venue_name, venue_country, congregation_name;

            // Extract the venue name
            getline(ss, venue_name, '\"');
            getline(ss, venue_name, '\"');

            // Extract the venue country
            ss >> ws; // Skip any whitespace
            getline(ss, venue_country, '\"');
            getline(ss, venue_country, '\"');

            // Extract the congregation name
            ss >> ws; // Skip any whitespace
            getline(ss, congregation_name, '\"');
            getline(ss, congregation_name, '\"');

            venue_helper.freeVenue(venue_name, venue_country, congregation_name, cong_helper.cong_list);
        }
        else if (action == "showReserved")
        {
            string congregation_name;

            // Extract the congregation name
            getline(ss, congregation_name, '\"');
            getline(ss, congregation_name, '\"');

            venue_helper.showReserved(congregation_name, cong_helper.cong_list);
        }
        else if (action == "addEvent")
        {
            string event_name, venue_name, country, date, start_time, end_time, details;

            // Extract the event name
            getline(ss, event_name, '\"');
            getline(ss, event_name, '\"');

            // Extract the venue name
            getline(ss, venue_name, '\"');
            getline(ss, venue_name, '\"');

            // Extract the country
            getline(ss, country, '\"');
            getline(ss, country, '\"');

            // Extract the date
            getline(ss, date, '\"');
            getline(ss, date, '\"');

            // Extract the start time
            getline(ss, start_time, '\"');
            getline(ss, start_time, '\"');

            // Extract the end time
            getline(ss, end_time, '\"');
            getline(ss, end_time, '\"');

            // Extract the additional details
            getline(ss, details, '\"');
            getline(ss, details, '\"');

            // Call addEvent with parsed values
            event_helper.addEvent(event_name, venue_name, country, date, start_time, end_time, details, cong_helper.cong_list, venue_helper.venues);
        }
        else if (action == "deleteEvent")
        {
            string event_name, venue_name, country, date, start_time, details;

            // Extract the event name
            getline(ss, event_name, '\"');
            getline(ss, event_name, '\"');

            // Extract the venue name
            getline(ss, venue_name, '\"');
            getline(ss, venue_name, '\"');

            // Extract the country
            getline(ss, country, '\"');
            getline(ss, country, '\"');

            // Extract the date
            getline(ss, date, '\"');
            getline(ss, date, '\"');

            // Extract the start time
            getline(ss, start_time, '\"');
            getline(ss, start_time, '\"');

            // Extract the additional details
            getline(ss, details, '\"');
            getline(ss, details, '\"');
            event_helper.deleteEvent(event_name, venue_name, country, date, start_time, details, cong_helper.cong_list, venue_helper.venues);
        }
        else if (action == "showEvents")
        {
            string venue_name, country, date;

            getline(ss, venue_name, '\"');
            getline(ss, venue_name, '\"');

            // Extract the country
            getline(ss, country, '\"');
            getline(ss, country, '\"');

            // Extract the date
            getline(ss, date, '\"');
            getline(ss, date, '\"');

            event_helper.showEvents(venue_name, country, date, cong_helper.cong_list, venue_helper.venues);
        }
        else if (action == "showCalendar")
        {
            string congregation_name, venue_name, country;
            getline(ss, congregation_name, '\"');
            getline(ss, congregation_name, '\"');

            getline(ss, venue_name, '\"');
            getline(ss, venue_name, '\"');

            getline(ss, country, '\"');
            getline(ss, country, '\"');

            event_helper.showCalendar(congregation_name, venue_name, country, cong_helper.cong_list, venue_helper.venues);
        }
        else if (action == "End")
        {
            break;
        }
        else
        {
            cout << "-1\nError\n";
        }
    }
}