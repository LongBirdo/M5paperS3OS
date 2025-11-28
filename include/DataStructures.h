#include <string>
#include <vector>

struct Weight{
    float weightnum;
};

struct History{
    std::string date;
    int sets;
    int reps;
    std::vector<Weight> weight;
};
struct Exercise {
    std::string name;
    int sets;
    int reps;
    std::string notes;
    std::vector<History> history;
    // TODO: You might want to parse 'details' into structured data later
    // float weight;
    // int reps;
    // int sets;
};

struct MuscleGroup {
    std::string name;
    std::vector<Exercise> exercises;
};

struct Workout {
    std::string name; 
    std::vector<MuscleGroup> muscleGroups;
};

struct LoggedExercise {
    std::string name;
};