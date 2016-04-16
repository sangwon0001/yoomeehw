// PL homework: hw1
// fsa.cc

#include <iostream>
#include <stdlib.h>
#include "fsa.h"

#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

const char kEps = '#';

using namespace std;
set getCombined(const set original, char input_char, const TableElement* elements, int num_elements)
{
	queue <int> temp_que;
	set <int> temp_set;
	int temp_num=0;
	int i;
	temp_que = original;
	if (input_char != '#') {
		while (!temp_que.empty()) {
			temp_num = temp_que.pop();
			for (i = 0; i < num_elements; i++) {
				if ((elements + i)->state == temp_num && (elements + i)->input_char == input_char) temp_que.push((elements + i)->next_state);
			}
		}
		temp_que = temp_set;
		temp_set.clear();
	}
	while (!temp_que.empty()) {
		temp_num = temp_que.pop();
		if (temp_set.find(temp_num) != temp_set.end()) continue;
		for (i = 0; i < num_elements; i++) {
			if ((elements + i)->state == temp_num && (elements + i)->input_char == '#') temp_que.push((elements + i)->next_state);
		}
		temp_set.insert(temp_num);
	}
	return temp_set;
}
bool CheckIfNFA(const TableElement* elements, int num_elements) {
  //TODO Implement this function.
	//(1)state 갯수 구하고, (2)알파벳 갯수 구하고,-->함수를 만들고 (3)#을 accept하는지 num_elements만큼 돌기(#accept하면 return true)
	vector <int> state_arr(3*num_elements);
	vector <int> char_arr(100);
	int i,j;
	int state_num = 0, char_num = 0;
	for (i = 0; i <= num_elements; i++) state_arr[i] = 0;
	for (i = 0; i <= 100; i++) char_arr[i] = 0;

	for (i = 0; i < num_elements; i++) {
		state_arr[(elements + i)->state]++;
		state_arr[(elements + i)->next_state]++;
		char_arr[(int)((elements + i)->input_char) - 40]++;
	}
	for (i = 0; i < num_elements; i++) if (state_arr[i] != 0) state_num++;
	for (i = 0; i <= 100; i++) if (char_arr[i] != 0) char_num++;
	for (i = 0; i < num_elements; i++) if ((elements + i)->input_char == '#') return true;//for문을 돌다가 중간에 나가면 nfa이다.
	//이차원 배열 만들기(state_num*alpabet_num)(check)

	vector < vector < int > > check;
	for (int i = 0; i <=state_num+1; i++)
	{
		vector<int> element;
		element.resize(char_num+1);
		check.push_back(element);
	}

	for (i = 0; i <= state_num; i++)
		for (j = 0; j <= char_num; j++)
			check[i][j] = -1;
	
	for (i = 0; i < num_elements; i++) check[(elements + i)->state][(elements + i)->input_char/*함수써주세용*/] = 1;
	//nfa인지 판별
	for (i = 1; i <= state_num; i++)
		for (j = 1; j <= char_num; j++)
			if (check[i][j] == -1) return true;
  return false;
}

bool BuildDFA(const TableElement* elements, int num_elements,
              const int* accept_states, int num_accept_states,
              FiniteStateAutomaton* fsa) {

  LOG << "num_elements: " << num_elements << endl;
  if (num_elements <= 0) return false;

  // Implement this function.
  //CheckIfNFA에서 (state_num*alpabet_num)(check)
  int i, j;
  vector <int> state_arr(num_elements);
  int char_arr[100];
  int i, j;
  int state_num = 0, char_num = 0;
  for (i = 0; i <= num_elements; i++) state_arr[i] = 0;
  for (i = 0; i <= 100; i++) char_arr[i] = 0;

  for (i = 0; i < num_elements; i++) {
	  state_arr[(elements + i)->state]++;
	  char_arr[(int)((elements + i)->input_char) - 40]++;
  }
  for (i = 0; i < num_elements; i++) if (state_arr[i] != 0) state_num++;
  for (i = 0; i <= 100; i++) if (char_arr[i] != 0) char_num++;

  for (i = 0; i < num_accept_states; i++) fsa->accept_states.insert(accept_states[i]);
  fsa->Transition_array = new int*[state_num];
  for (i = 0; i <= state_num; i++) fsa->Transition_array[i] = new int[char_num];

  for (i = 0; i <= state_num; i++)
	  for (j = 0; j <= char_num; j++)
		  fsa->Transition_array[i][j] = -1;

  for (i = 0; i < num_elements; i++) fsa->Transition_array[(elements + i)->state][(elements + i)->input_char] = (elements + i)->next_state;
  


  return false;
}
struct TableElement2 {
	set <int> state;
	char input_char;
	set <int> next_state;
};
bool BuildNFA(const TableElement* elements, int num_elements,
	const int* accept_states_array, int num_accept_states,
	FiniteStateAutomaton* fsa) {
	// Implement this function.

	  //state갯수와 char갯수 구하기
	vector <int > state_arr(num_elements);
	int char_arr[100];
	int i, j;
	int state_num = 0, char_num = 0, char_num_include_shop = 0;
	for (i = 0; i <= num_elements; i++) state_arr[i] = 0;
	for (i = 0; i <= 100; i++) char_arr[i] = 0;

	for (i = 0; i < num_elements; i++) {
		state_arr[(elements + i)->state]++;
		state_arr[(elements + i)->next_state]++;
		if ((elements + i)->input_char == '#') char_arr[0]++;
		else char_arr[(int)((elements + i)->input_char) - 40]++;
	}
	for (i = 0; i < num_elements; i++) if (state_arr[i] != 0) state_num++;
	if (char_arr[0] == 0/*#이 없다*/) {
		for (i = 1; i <= 100; i++) if (char_arr[i] != 0) char_num++;
		char_num_include_shop = char_num;
	}
	else {
		for (i = 0; i <= 100; i++) if (char_arr[i] != 0) char_num++;
		char_num_include_shop = char_num;
		char_num--;
	}
	//char_num은 #없는거, char_num_include_shop은 #이 있으면 1이 더 크다.
	//for (i = 0; i < num_accept_states; i++) fsa->accept_states.insert(accept_states[i]);

	//nfa->dfa
	vector < set <int> > combined_states;
	queue < set <int> > combined_queue;
	set < int > temp_set, temp_set2;
	temp_set2.insert(1);
	temp_set = getCombined(temp_set2, '#', elements);
	combined_queue.push(temp_set);

	vector<TableElement2*>  fsa_element;
	while (!combined_queue.empty()) {
		temp_set = combined_queue.pop();
		if (isInVec(combined_states, temp_set) == 1/*set이 벡터 안에 있다*/) continue;
		for (i = 0; i < char_num; i++/*여기서 char_num은 #을 뺀 갯수*/) {
			temp_set2 = getCombined(temp_set, char_arr[i], elements);
			combined_queue.push(temp_set2);
			struct TableElement2 temp_element2;
			temp_element2.input_char = char_arr[i];
			temp_element2.state = temp_set;
			temp_element2.next_state = temp_set2;
			fsa_element.push_back(temp_element2);
		}
		combined_states.push_back(temp_set);
	}
	//fsa_element
	vector<TableElement> *fsa_element2;//NFA를 DFA로 바꾼 다음 새로 만든 transition;
	for (i = 0; i < fsa_element.size(); i++) {
		struct TableElement temp_element;
		temp_element.state = getIndexOf(fsa_element[i]->state, combined_states);
		temp_element.input_char = fsa_element[i]->input_char;
		temp_element.next_state = getIndexOf(fsa_element[i]->state, combined_states);
		fsa_element2->push_back(temp_element);
	}
	vector <int > new_accept_array;
	for (i = 0; i < combined_states.size(); i++) {
		for (j = 0; j < num_accept_states; j++) {
			if (combined_states[i].find(accept_array[j]) != combined_state[i].end()) {
				new_accept_array.push_back(i + 1);
				break;
			}
		}
	}
	return BuildDFA(&fsa_element2[0], fsa_element2->size(),
		&new_accept_array[0], new_accept_array.size());
}

// Homework 1.1
bool RunFSA(const FiniteStateAutomaton* fsa, const char* str) {
  // Implement this function.
  return false;
}

// Homework 1.1 and 1.2
bool BuildFSA(const TableElement* elements, int num_elements,
              const int* accept_states, int num_accepts,
              FiniteStateAutomaton* fsa) {
  if (CheckIfNFA(elements, num_elements)) {
    return BuildNFA(elements, num_elements, accept_states, num_accepts, fsa);
  } else {
    return BuildDFA(elements, num_elements, accept_states, num_accepts, fsa);
  }
}
// Homework 1.3
bool BuildFSA(const char* regex, FiniteStateAutomaton* fsa) {
  // Implement this function.
  return false;
}

