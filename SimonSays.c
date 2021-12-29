#define BUTTON_BLINK


#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions
#include <time.h>

#include "ece198.h"

void rand_color(char *array, int level_size);
bool levels ();
void getInput (char*lvl, int level_size, char*input);
void decode (char *lvl, int level_size);


void main(void) {
    srandom((unsigned int) time(NULL));
    HAL_Init(); 
    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example)
    InitializePin(GPIOB, GPIO_PIN_5 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, 0); 
    SerialSetup(9600);

    char buff[100];
    sprintf(buff, "hello");
    SerialPuts(buff);


    levels ();
    
    char buttons=0;

    }

void decode (char *lvl, int level_size) {
    for (int i=0; i<level_size; i++) {
        if (lvl[i] == 'y') {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
            HAL_Delay(1000);
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
            char buff[100];
            sprintf(buff, "printed yellow\n");
            SerialPuts(buff);
        }
        else if (lvl[i] == 'g') {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
            HAL_Delay(1000);
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
            char buff[100];
            sprintf(buff, "printed green\n");
            SerialPuts(buff);
        }
        else if (lvl[i] == 'b') {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
            HAL_Delay(1000);
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
            char buff[100];
            sprintf(buff, "printed blue\n");
            SerialPuts(buff);
        }
        else if (lvl[i] == 'r') {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
            HAL_Delay(1000);
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
            char buff[100];
            sprintf(buff, "printed red\n");
            SerialPuts(buff);
        }
    }  

}
void rand_color(char *array, int level_size) {
    char colors[4] = {'b', 'r', 'y', 'g'};
    for(int i=0; i<level_size; i++) {
        array[i] = colors[random() % 4 ];
    }
}

bool levels () {
    char *lvl=0;
    char *input=0;
    int level_size = 3;
    while (level_size != 6) {
        lvl = (char*) malloc(level_size*sizeof(int));
        input = (char*) malloc(level_size*sizeof(int));
        
        rand_color(lvl, level_size);
        char buff[100];
        sprintf(buff, "under rand_color\n");
        SerialPuts(buff);
        decode (lvl, level_size);
        getInput(lvl, level_size, input);

        bool pass = true;
        for (int i=0; i<level_size; i++) {
            if (input[i] != lvl[(level_size-1-i)]) {
                level_size = 3;
                bool pass = false;
            } 
        }
        if (pass == true) {
            level_size++;
        }
        
    }
    free(lvl);
    free(input);
    for (int i=0; i<3; i++) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        HAL_Delay(500);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        HAL_Delay(500);
    }
    return true;
}

void getInput(char*lvl, int level_size, char*input){
    for (int i=0; i < level_size; i++) {
        while (! ( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) || HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0))) {
        }
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
            input[i] = 'y';
            char buff[100];
            sprintf(buff, "yellow\n");
            SerialPuts(buff);
        } if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)) {
            input[i] = 'g';
            char buff[100];
            sprintf(buff, "green\n");
            SerialPuts(buff);
        } if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)) {
            input[i] = 'b';
            char buff[100];
            sprintf(buff, "blue\n");
            SerialPuts(buff);
        } if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)) {
            input[i] = 'r';
            char buff[100];
            sprintf(buff, "red\n");
            SerialPuts(buff);
        }
        while (( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) || HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0))) {
        }
    }

}
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
}
