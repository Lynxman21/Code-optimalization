import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# 1. Funkcja pomocnicza do zamiany formatu 0:04.17 na sekundy (float)
def time_to_seconds(t_str):
    try:
        minutes, seconds = t_str.split(':')
        return int(minutes) * 60 + float(seconds)
    except:
        return float(t_str)

# 2. Wczytanie danych
df = pd.read_csv('wyniki_pomiarow.csv')

# Czyszczenie danych
df['Czas_s'] = df['Czas_Real[s]'].apply(time_to_seconds)
df['Max_RAM_MB'] = df['Max_RAM[KB]'] / 1024

# Skupiamy się na największym pliku, żeby różnice były wyraźne
# (Możesz to zmienić na dowolny inny plik z Twojej listy)
target_file = 'tests/test-500kB.txt'
df_target = df[df['Plik'] == target_file].copy()

# 3. Ustawienie stylu wykresów
sns.set_theme(style="whitegrid")
plt.rcParams.update({'font.size': 12})

# --- WYKRES 1: Czas Wykonania ---
plt.figure(figsize=(10, 6))
plot1 = sns.barplot(data=df_target, x='Program', y='Czas_s', palette='viridis')
plt.title(f'Czas wykonania dla pliku: {target_file}')
plt.ylabel('Czas [sekundy]')
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig('wykres_czasu.png')

# --- WYKRES 2: Utylizacja CPU (Dowód na OpenMP) ---
plt.figure(figsize=(10, 6))
plot2 = sns.barplot(data=df_target, x='Program', y='CPU_Percent', palette='magma')
plt.axhline(100, color='red', linestyle='--', label='100% (1 rdzeń)')
plt.title('Wykorzystanie mocy procesora (CPU %)')
plt.ylabel('Utylizacja CPU [%]')
plt.legend()
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig('wykres_cpu.png')

# --- WYKRES 3: Zużycie RAM ---
plt.figure(figsize=(10, 6))
plot3 = sns.barplot(data=df_target, x='Program', y='Max_RAM_MB', palette='rocket')
plt.title('Maksymalne zużycie pamięci RAM')
plt.ylabel('Pamięć [MB]')
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig('wykres_ram.png')

# Filtrujemy dane tylko dla bare-metal
df_bare = df[df['Program'] == 'exe/normalize5.out'].copy()

# Wykres 1: Skalowanie (tylko dla plików z rozmiarem w nazwie)
# Musisz ręcznie przypisać rozmiary, jeśli nie masz ich w CSV
rozmiary = {'tests/test-100kB.txt': 100, 'tests/test-250kB.txt': 250, 'tests/test-500kB.txt': 500}
df_bare_scale = df_bare[df_bare['Plik'].isin(rozmiary.keys())].copy()
df_bare_scale['Size'] = df_bare_scale['Plik'].map(rozmiary)

plt.figure(figsize=(10, 5))
sns.lineplot(data=df_bare_scale, x='Size', y='Czas_s', marker='o', color='green')
plt.title('Skalowanie czasowe wersji Bare-metal')
plt.xlabel('Rozmiar pliku [kB]')
plt.ylabel('Czas [s]')
plt.savefig('baremetal_scaling.png')

# Wykres 2: Porównanie typów plików
plt.figure(figsize=(10, 7))
sns.barplot(data=df_bare, x='Plik', y='Czas_s', palette='coolwarm')
plt.xticks(rotation=20)
plt.title('Wydajność Bare-metal dla różnych typów danych')
plt.savefig('baremetal_files_comparison.png')

print("Wykresy zostały wygenerowane i zapisane w folderze projektu!")