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

const size_t height = 22;                               // Высота поля (-2) -> 20
const size_t width  = 22;                               // Ширина поля (-2) -> 20

// Структура живой клетки
struct LiveCell {
  /*
    0 - мертвая
    1 - пустая
    2 - доп граница (установлена вокруг поля)
  */
  size_t is_alive = 0;
};

// Генерация рандомной карты (двумерный вектор, элементы которого объекты структуры с одним полем - описанием состояния клетки)
vector<vector<LiveCell>> createMap(const size_t height, const size_t width) {
  vector<vector<LiveCell>> map; srand(time(nullptr));
  
  for (size_t row = 0; row < height; row++) {
    map.push_back({});

    for (size_t col = 0; col < width; col++) {
      if (row * col == 0 || row == height - 1 || col == width - 1) {
        LiveCell temp; temp.is_alive = 2;
        map[row].push_back(temp);
      } else {
        if (rand() % 10 > 7) {
          LiveCell temp; temp.is_alive = 1;
          map[row].push_back(temp);
        } else {
          LiveCell temp;
          map[row].push_back(temp);
        }
      }
    }
  }  

  return map;
}

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
  vector<vector<size_t>> coord_pairs;
  
  if (map[x+1][y+1].is_alive == 1) { coord_pairs.push_back({x+1, y+1}); }
  if (map[x-1][y-1].is_alive == 1) { coord_pairs.push_back({x-1, y-1}); }
  if (map[x+1][y-1].is_alive == 1) { coord_pairs.push_back({x+1, y-1}); }
  if (map[x-1][y+1].is_alive == 1) { coord_pairs.push_back({x-1, y+1}); }
  if (map[x][y+1].is_alive == 1)   { coord_pairs.push_back({x, y+1}); }
  if (map[x][y-1].is_alive == 1)   { coord_pairs.push_back({x, y-1}); }
  if (map[x+1][y].is_alive == 1)   { coord_pairs.push_back({x+1, y}); }
  if (map[x-1][y].is_alive == 1)   { coord_pairs.push_back({x-1, y}); }
  
  return coord_pairs.size();
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
  auto map = createMap(height, width);  // Генерируем поле 20x20
  LiveCell c_1, c_2, c_0;
  c_1.is_alive = 1; c_2.is_alive = 2; c_0.is_alive = 0;
  
  map = {
    {c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_1, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_1, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_1, c_1, c_1, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_0, c_2},
    {c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2, c_2},
  };

  printMap(map); Sleep(1000);

  while (true) {
      cls;
      auto prev_map = map;
      map = nextGeneration(map);
      printMap(map);
      
      if (!getAliveCount(map) || compareMaps(map, prev_map)) {
        over;
        return 0;
      }

      Sleep(250);
  }
}