from googletrans import Translator

inputFile = open("kaz_text.txt", "r")
translator = Translator()
eng = translator.translate(inputFile.read()).text
inputFile.close()

outputFile = open("eng_text.txt", "w")
outputFile.write(eng)
outputFile.close()
print(eng)
