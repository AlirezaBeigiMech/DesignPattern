
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration
struct Subject;

// Observer structure
typedef struct Observer {
    void (*update)(struct Observer *self, int value);
} Observer;

// Subject structure
typedef struct Subject {
    Observer **observers;
    int observer_count;
    int state;
} Subject;

void attach(Subject *subject, Observer *observer) {
    subject->observers[subject->observer_count++] = observer;
}

void notify(Subject *subject) {
    for (int i = 0; i < subject->observer_count; ++i) {
        subject->observers[i]->update(subject->observers[i], subject->state);
    }
}

void setState(Subject *subject, int state) {
    subject->state = state;
    notify(subject);
}

typedef struct {
    Observer base; // Inherit from Observer
    char *name;
} ConcreteObserver;

void updateObserver(Observer *observer, int value) {
    // Cast back to ConcreteObserver to access additional data
    ConcreteObserver *self = (ConcreteObserver *)observer;
    printf("%s received notification: State changed to %d\n", self->name, value);
}

ConcreteObserver *newConcreteObserver(const char *name) {
    ConcreteObserver *obj = malloc(sizeof(ConcreteObserver));
    obj->name = strdup(name);
    obj->base.update = updateObserver;
    return obj;
}

int main() {
    // Initialize subject
    Subject mySubject = {0};
    mySubject.observers = malloc(sizeof(Observer*) * 10); // Example, in real use, manage memory and scaling better
    mySubject.observer_count = 0;

    // Create observers
    ConcreteObserver *observer1 = newConcreteObserver("Observer 1");
    ConcreteObserver *observer2 = newConcreteObserver("Observer 2");

    // Attach observers to subject
    attach(&mySubject, (Observer *)observer1);
    attach(&mySubject, (Observer *)observer2);

    // Change state of subject to trigger notifications
    setState(&mySubject, 10);
    setState(&mySubject, 20);

    // Cleanup
    free(observer1);
    free(observer2);
    free(mySubject.observers);

    return 0;
}