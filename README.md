# Minicron
Prosty harmonogram zadań wzorowany na linuxowym cronie

### Użycie
```minicron nazwa_pliku```

## Struktura pliku
```
<hour>:<minutes>:<task1>:<logLevel>
<hour>:<minutes>:<task2>:<logLevel>
```
`hour` `minutes` godzina wykonania zadania  
`taskX` zadanie do wykonania  
`logLevel` określa poziom zapisywania danych do logu systemowego zwracanych na standardowe wyjscia przez aplikację:
- `0`  standardowe wyjście (stdout)  
- `1`  na wyjście błędów (stderr).  
- `2`  standardowe wyjście i wyjście błędów.  

## Funkcjonalności
- Demon wczytuje zadania z pliku i porządkuje je chronologicznie, a następnie zasypia. Budzi się, kiedy nadejdzie czas wykonania pierwszego zadania, tworzy proces potomny wykonujący zadanie, a sam znowu zasypia do czasu kolejnego zadania. Proces potomny wykonuje zadanie uruchamiając nowy proces. Zadania wykonywane są cyklicznie.
- Demon kończy pracę po otrzymaniu sygnału SIGINT i ewentualnym zakończeniu bieżącego zadania (zadań).
- Informacje o uruchomieniu zadania i kodzie wyjścia zakończonego zadania umieszczane są w logu systemowym.
- Po otrzymaniu sygnału SIGUSR1 demon ponownie wczytuje zadania z pliku, porzucając ewentualne nie wykonane jeszcze zadania.
- Po otrzymaniu sygnału SIGUSR2 demon wpisuje do logu systemowego listę zadań, jakie pozostały do wykonania. Jeśli wszystkie zadania zostały wykonane co najmniej raz, w logu zapisywana jest stosowna informacja.
- W zależności podanego w pliku konfiguracyjnym prarametru logLevel standardowe wyjścia aplikacji są przekierowywane do logu systemowego.

## Budowanie projektu

## Testowanie