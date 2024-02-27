#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a common interface
typedef struct TextComponent {
    char *(*getText)(struct TextComponent *self);
    void (*destroy)(struct TextComponent *self);
} TextComponent;

typedef struct {
    TextComponent base;
    char *text;
} SimpleText;

char *getTextSimpleText(TextComponent *self) {
    SimpleText *obj = (SimpleText *)self;
    return obj->text;
}

void destroySimpleText(TextComponent *self) {
    free(self);
}

SimpleText *newSimpleText(const char *text) {
    SimpleText *obj = malloc(sizeof(SimpleText));
    obj->text = strdup(text);
    obj->base.getText = getTextSimpleText;
    obj->base.destroy = destroySimpleText;
    return obj;
}


typedef struct {
    TextComponent base;
    TextComponent *wrapped;
    char *prefix;
} PrefixDecorator;

char *getTextPrefixDecorator(TextComponent *self) {
    PrefixDecorator *obj = (PrefixDecorator *)self;
    static char buffer[1024]; // Example purposes; in real use, manage memory better
    sprintf(buffer, "%s%s", obj->prefix, obj->wrapped->getText(obj->wrapped));
    return buffer;
}

void destroyPrefixDecorator(TextComponent *self) {
    PrefixDecorator *obj = (PrefixDecorator *)self;
    obj->wrapped->destroy(obj->wrapped);
    free(obj);
}

PrefixDecorator *newPrefixDecorator(TextComponent *wrapped, const char *prefix) {
    PrefixDecorator *obj = malloc(sizeof(PrefixDecorator));
    obj->wrapped = wrapped;
    obj->prefix = strdup(prefix);
    obj->base.getText = getTextPrefixDecorator;
    obj->base.destroy = destroyPrefixDecorator;
    return obj;
}


int main() {
    // Create a simple text component
    SimpleText *simpleText = newSimpleText("Hello, World!");

    // Decorate the simple text with a prefix
    PrefixDecorator *decoratedText = newPrefixDecorator((TextComponent *)simpleText, "Prefix: ");

    // Use the decorated text
    printf("%s\n", decoratedText->base.getText((TextComponent *)decoratedText));

    // Clean up
    decoratedText->base.destroy((TextComponent *)decoratedText);

    return 0;
}