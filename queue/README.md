# queue
Implementacja kolejki cyklicznej oraz parsowania pliku zadań.


## queue.c
`void addTask()` dodaje zadanie do kolejki, przy dodawaniu zachowywana jest chronologia zadań  
`task *getTask()` zwaraca kolejne zadanie z kolejki, jednocześnie to zadanie jest przesuwane na koniec kolejki  
`int compareElements()` porównuje podane jako argumenty zadania po czasie ich wykonania:  
    zwraca 1 gdy zadanie a jest zaplanowane na późniejszą godzinę niż zadanie b  
`void queueDestroy()` dealokacji pamieci zajmowanej przez kolejkę  
`taskQueue *createQueue()` inicjalizacja struktury kolejki  
`void rotateQueue()` przesuwa kolejka, tak że getTask() zwraca kolejne zadania do wykonania. Funkcje należy wykonać po sparsowaniu pliku.
`void printQueue()` wypisanie wszystkich zadań z kolejki  

## read.c
`taskQueue *readFile()` tworzy kolejkę zadań na podstawie pliku
