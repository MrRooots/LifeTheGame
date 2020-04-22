#include <iostream>
#include <vector>
#include <windows.h>
#include <ctime>
using namespace std;

#define cls system("cls");
#define over cout << "Game over!" << endl;

/*
Место действия этой игры — «вселенная» — это размеченная на клетки плоскость
Каждая клетка на этой поверхности может находиться в двух состояниях: быть заполненной или быть пустой. 
Клетка имеет восемь соседей, окружающих её.
Распределение живых клеток в начале игры называется первым поколением. 
Каждое следующее поколение рассчитывается на основе предыдущего по таким правилам:
  в пустой клетке, рядом с которой ровно три живые клетки, зарождается жизнь;
  если у живой клетки есть две или три живые соседки, то эта клетка продолжает жить; 
  в противном случае, если соседей меньше двух или больше трёх, клетка умирает
Игра прекращается, если
  на поле не останется ни одной «живой» клетки
  при очередном шаге ни одна из клеток не меняет своего состояния (складывается стабильная конфигурация; предыдущее правило, вырожденное до одного шага назад)
*/

size_t height;                               // Высота поля (-2) -> 20
size_t width;                               // Ширина поля (-2) -> 20

// Структура живой клетки
struct LiveCell {
  /*
    0 - мертвая
    1 - пустая
    2 - доп граница (установлена вокруг поля)
  */
  size_t is_alive = 0;
};

vector<vector<LiveCell>> createEmptyMap(const size_t height, const size_t width) {
  vector<vector<LiveCell>> map;
  
  for (size_t row = 0; row < height; row++) {
    map.push_back({});

    for (size_t col = 0; col < width; col++) {
      if (row * col == 0 || row == height - 1 || col == width - 1) {
        LiveCell temp; temp.is_alive = 2;
        map[row].push_back(temp);
      } else {
        LiveCell temp;
        map[row].push_back(temp);
      }
    }
  }  

  return map;
}

// Печать карты
template<typename T>
void printMap(T const& map) {
  for (auto row : map) {
    for (auto cell : row) {
      if (cell.is_alive == 1) {
        cout << 'o';
      } else if (cell.is_alive != 2){
        cout << '.';
      }
    } 
    if (row[1].is_alive != 2) {
      cout << endl;
    }
  }
}

// Подсчет кол-ва живых клеток
template<typename T>
size_t getAliveCount(T const& map) {
  size_t count = 0;

  for (auto layer : map) {
    for (auto cell : layer) {
      if (cell.is_alive == 1) {
        count++;
      }
    }
  }
  
  return count;
}

// Вернет все координаты соседних клеток к полученной
template<typename T>
size_t getAllNeighbours(T const& map, const size_t x, const size_t y) {
  size_t count = 0;
  
  if (map[x+1][y+1].is_alive == 1) { count++; }
  if (map[x-1][y-1].is_alive == 1) { count++; }
  if (map[x+1][y-1].is_alive == 1) { count++; }
  if (map[x-1][y+1].is_alive == 1) { count++; }
  if (map[x][y+1].is_alive == 1)   { count++; }
  if (map[x][y-1].is_alive == 1)   { count++; }
  if (map[x+1][y].is_alive == 1)   { count++; }
  if (map[x-1][y].is_alive == 1)   { count++; }
  
  return count;
}

template<typename T>
T nextGeneration(T & map) {
  T new_map = createEmptyMap(height, width);

  for (size_t y = 1; y < height - 1; y++) {
    for (size_t x = 1; x < width - 1; x++) {
      const size_t result = getAllNeighbours(map, y, x);

      if (map[y][x].is_alive == 0 && result == 3) {  // Зарождается жизнь
        new_map[y][x].is_alive = 1;
      } else if (map[y][x].is_alive == 1 && (result > 3 || result < 2)) {  // Клетка умирает
        new_map[y][x].is_alive = 0;
      } else {
        new_map[y][x] = map[y][x];
      }
    }
  }

  return new_map;
}

template<typename T>
bool compareMaps(T const& current_map, T const& prev_map) {
  for (size_t i = 0; i < current_map.size(); i++) {
    for (size_t l = 0; l < current_map.size(); l++) {
      if (current_map[i][l].is_alive != prev_map[i][l].is_alive) {
        return false;
      }
    }    
  }
  return true;
}

int main() {
  // auto map = createMap(height, width);  // Генерируем поле 20x20
  LiveCell c1, c2, c0;
  c1.is_alive = 1; c2.is_alive = 2; c0.is_alive = 0;
  
  vector<vector<LiveCell>> map;
  // 5 вариант получился совершенно случайно :0
  size_t choice; cout << "1) Glider\n2) Oscillator\n3) Oscillator v2\n4) Blow\n5) Huge Blow -> static\nYour choice: "; cin >> choice;
  
  // Selection
  if (choice == 1) {
    map = {
      {c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c1, c1, c1, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2},
    };
    height = 12; width = 12;
  } else if (choice == 2) {
    map = {
      {c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c1, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c1, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c1, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2},
    };
    height = 12; width = 12;
  } else if (choice == 3) {
    map = {
      {c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c1, c1, c0, c1, c1, c1, c1, c0, c1, c1, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2},
    };
    height = 20; width = 20;
  } else if (choice == 4) {
    map = {
      {c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c1, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c1, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2}
    };
    height = 30; width = 30;
  } else if (choice == 5) {
    // Не открывать!
    map = {
      {c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c1, c0, c1, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c1, c0, c1, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c1, c0, c1, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c1, c0, c1, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c1, c0, c1, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c1, c0, c1, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c0, c0, c1, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c1, c0, c1, c0, c1, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c1, c0, c1, c0, c1, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c0, c2},
      {c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2, c2},
    };
    height = 32; width = 40;
  }

  printMap(map); Sleep(1000);

  while (true) {
    cls;

    auto prev_map = map;
    map = nextGeneration(map);
    printMap(map);
    
    if (!getAliveCount(map) || compareMaps(map, prev_map)) { over; return 0; }
    Sleep(100);
  }
}