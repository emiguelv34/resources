#include <iostream>
#include <map>
#include "Schedule.h"

using namespace std;

Schedule::Schedule(string name)
{
    mName = name;
}

bool Schedule::dropCourse(std::string name)
{
    // TO DO: if the course was not already in the map, return false
    // otherwise, remove it from the map and return true!
    std::map<std::string, Course*>::iterator it = courseMap.find(name);
    if(it != courseMap.end()){
        courseMap.erase(it);
        return true;
    }
    return false;
}

bool Schedule::addCourse(Course* c)
{
    // TO DO: if the course was already in the map, return false
    // otherwise, add it to the map and return true!
    std::string courseName = c->getCourseName();
    if(courseMap.find(courseName) != courseMap.end()){
        return false;
    }
    courseMap[courseName] = c;
    return true;
}

void Schedule::printAllAssignments()
{
    // TO DO: print out the name of each course, followed by the set of assignments.
    // for each assignment, display its name, its type (essay vs hw vs exam), and whether or not it was completed.
    // formatting is up to you, but make sure it's easy to read!
    for(std::map<std::string , Course*>::iterator Courseit = courseMap.begin(); Courseit != courseMap.end(); ++Courseit){
        std::string courseName = Courseit->first;
        Course* course = Courseit->second;
        std::cout << "Course: " << courseName << std::endl;

        std::set<Assignment*> assignments = course->getAssignmentSet();

        for (Assignment* assignment : assignments)
        {
            std::cout << "Assignment: " << assignment->getName() << std::endl;
            std::cout << "Type: " << assignment->getType() << std::endl;
            std::cout << "Completed: ";
            if (assignment->isComplete()){
                std::cout << "Yes" << std::endl;
            }
            else{
                std::cout << "No" << std::endl;
            }
        }
    }
}

std::map<std::string, Course*> Schedule::getCourses()
{
    return courseMap;
}