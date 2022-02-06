#include <iostream>
#include <string>

class Sport {
public:
    Sport(std::string sportName) : sportName(sportName) {}
    std::string name() const { return sportName; };
    virtual bool isOutdoor() const = 0;
    virtual std::string icon() const = 0;
    void destructorMsg(std::string className, std::string addendum = "") const {
        std::cout << "Destroying the " << className
                  << " object named " << name()
                  << addendum << "." << std::endl;
    }
    virtual ~Sport() {}

private:
    std::string sportName;
};

class Snowboarding : public Sport {
public:
    Snowboarding(std::string sportName) : Sport(sportName) {}
    bool isOutdoor() const { return true; }
    std::string icon() const { return "a descending snowboarder"; }
    ~Snowboarding() { destructorMsg("Snowboarding"); }
};

class Biathlon : public Sport {
public:
    Biathlon(std::string sportName, double distance) : Sport(sportName),
                                                       distance(distance) {}
    bool isOutdoor() const { return true; }
    std::string icon() const { return "a skier with a rifle"; }
    ~Biathlon() {
        std::string distStr = std::to_string(distance);
        distStr.erase(distStr.find_last_not_of('0') + 1);
        destructorMsg("Biathlon", ", distance " + distStr + " km");
    }

private:
    double distance;
};

class FigureSkating : public Sport {
public:
    FigureSkating(std::string sportName) : Sport(sportName) {}
    bool isOutdoor() const { return false; }
    std::string icon() const { return "a skater in the Biellmann position"; }
    ~FigureSkating() { destructorMsg("FigureSkating"); }
};
