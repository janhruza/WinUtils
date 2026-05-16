#pragma once
#ifndef ACTIONS_H
#define ACTIONS_H

void CopyToClipboard(const wchar_t* content);

void ACClearHistory();
void ACAddToHistory(const wchar_t* text);

#endif