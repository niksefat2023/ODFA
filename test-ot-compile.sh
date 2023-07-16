#Oblivious DFA Code By:
#Saeed Sadeghian (sadeghis@gmail.com)
#Salman Niksefat (niksefat@aut.ac.ir)

g++ -L /usr/local/lib/ -lcppsocket hr_time.cpp EC_Naor_OT.cpp  test-ot.cpp Experiments.cpp OAE.cpp DFA.cpp cryptopp-lib/*.o -o test-ot
