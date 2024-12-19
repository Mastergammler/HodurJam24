// because of redefinition of default argument
#pragma once

const float BEFORE_NUM = .65;

void PlayNumberSound(int number, float delay = 0);
void Ui_HandleInputs();
void OnUiEnter();
void OnUiExit();
