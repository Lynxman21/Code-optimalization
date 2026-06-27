import random

def generate_test(name, size, mode):
    size = size * 1024 # from kB to B
    saved = 0

    words = ["agh", "optymalizacja", "student", "kod", "cplusplus", "test", "program", "AGH", "Optymalizacja", "Student", "Kod", "Cplusplus", "Test", "Program"]
    punctuation = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
    w_chars = [" ", " ", " ", "\n", "\n", "\t"]

    with open(name, 'w', encoding='utf-8') as f:
        while saved < size:
            part = ""
            elements = []
            if mode == "normal":
                for _ in range(100):
                    w = random.choice(words)
                    if random.random() < 0.2:
                        w += random.choice(punctuation)
                    elements.append(w)
                
                part = " ".join(elements) + " "
            elif mode == "spaces":
                for _ in range(100):
                    elements.append(random.choice(words))
                    elements.append(random.choice(w_chars) * random.randint(1,3))
                part = "".join(elements)
            elif mode == "punct":
                for _ in range(100):
                    w = random.choice(words)
                    w += "".join(random.choices(punctuation, k=random.randint(1,3)))
                    elements.append(w)
                part = "".join(elements) + " "
            elif mode == "duplicates":
                w = random.choice(words)
                part = (w + " ") * 500
            
            part += "\n"

            f.write(part)
            saved += len(part.encode('utf-8'))
        
    print(f"Zakończone generowanie ${name}")
    
generate_test("test-100kB.txt", 100, "normal")
generate_test("test-250kB.txt", 250, "normal")
generate_test("test-500kB.txt", 500, "normal")

generate_test("test-spaces.txt", 250, "spaces")
generate_test("test-punct.txt", 250, "punct")
generate_test("test-duplicates.txt", 250, "duplicates")

print("Koniec")