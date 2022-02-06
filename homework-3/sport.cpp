class Sport {
public:
    Sport(string sportName) : sportName(sportName) {}
    string name() const {
        return sportName;
    };
    virtual bool isOutdoor() const = 0;
    virtual string icon() const = 0;
    void destructorMsg(string className, string addendum = "") const {
        cout << "Destroying the " << className
             << " object named " << name()
             << addendum << "." << endl;
    }
    virtual ~Sport() {}

private:
    string sportName;
};

class Snowboarding : public Sport {
public:
    Snowboarding(string sportName) : Sport(sportName) {}
    bool isOutdoor() const {
        return true;
    }
    string icon() const {
        return "a descending snowboarder";
    }
    ~Snowboarding() {
        destructorMsg("Snowboarding");
    }
};

class Biathlon : public Sport {
public:
    Biathlon(string sportName, double distance) : Sport(sportName),
                                                  distance(distance) {}
    bool isOutdoor() const {
        return true;
    }
    string icon() const {
        return "a skier with a rifle";
    }
    ~Biathlon() {
        string distStr = to_string(distance);
        distStr.erase(distStr.find_last_not_of('0') + 1);
        destructorMsg("Biathlon", ", distance " + distStr + " km");
    }

private:
    double distance;
};

class FigureSkating : public Sport {
public:
    FigureSkating(string sportName) : Sport(sportName) {}
    bool isOutdoor() const {
        return false;
    }
    string icon() const {
        return "a skater in the Biellmann position";
    }
    ~FigureSkating() {
        destructorMsg("FigureSkating");
    }
};
