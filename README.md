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
`logLevel` określa poziom zapisywania danych do logu systemowego zwracanych na standarodwe wyjscia przez aplikację:
- `0`  użytkownik chce otrzymać treść, jaką polecenie wypisało na standardowe wyjście (stdout)  
- `1`  użytkownik chce otrzymać treść, jaką polecenie wypisało na wyjście błędów (stderr).  
- `2`  użytkownik chce otrzymać treść, jaką polecenie wypisało na standardowe wyjście i wyjście błędów.  

## queue.c

## read.c