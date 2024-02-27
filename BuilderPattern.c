#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *dough;
    char *sauce;
    char *topping;
} Pizza;

typedef struct PizzaBuilder {
    void (*setDough)(struct PizzaBuilder* builder, const char* dough);
    void (*setSauce)(struct PizzaBuilder* builder, const char* sauce);
    void (*setTopping)(struct PizzaBuilder* builder, const char* topping);
    Pizza* (*build)(struct PizzaBuilder* builder);
    Pizza* pizza;
} PizzaBuilder;

void setDough(PizzaBuilder* builder, const char* dough) {
    builder->pizza->dough = strdup(dough);
}

void setSauce(PizzaBuilder* builder, const char* sauce) {
    builder->pizza->sauce = strdup(sauce);
}

void setTopping(PizzaBuilder* builder, const char* topping) {
    builder->pizza->topping = strdup(topping);
}

Pizza* build(PizzaBuilder* builder) {
    return builder->pizza;
}

PizzaBuilder* newPizzaBuilder() {
    PizzaBuilder* builder = (PizzaBuilder*)malloc(sizeof(PizzaBuilder));
    builder->pizza = (Pizza*)malloc(sizeof(Pizza));
    builder->setDough = setDough;
    builder->setSauce = setSauce;
    builder->setTopping = setTopping;
    builder->build = build;
    return builder;
}

void deletePizzaBuilder(PizzaBuilder* builder) {
    if (builder->pizza) {
        free(builder->pizza->dough);
        free(builder->pizza->sauce);
        free(builder->pizza->topping);
        free(builder->pizza);
    }
    free(builder);
}
int main() {
    PizzaBuilder* pizzaBuilder = newPizzaBuilder();
    pizzaBuilder->setDough(pizzaBuilder, "Thick crust");
    pizzaBuilder->setSauce(pizzaBuilder, "Tomato");
    pizzaBuilder->setTopping(pizzaBuilder, "Pepperoni");

    Pizza* myPizza = pizzaBuilder->build(pizzaBuilder);
    printf("Pizza with %s, %s and %s\n", myPizza->dough, myPizza->sauce, myPizza->topping);

    // Clean up
    deletePizzaBuilder(pizzaBuilder);

    return 0;
}