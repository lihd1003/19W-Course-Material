del /f test_result_e3.txt

echo ----ADD RESTAURANTS----
node app.js --addRest "Red Lobster" "Seafood at low prices" >> test_result_e3.txt
node app.js --addRest "Red Lobster" "Duplicate" >> test_result_e3.txt
node app.js --addRest "Blue Lobster" "Low prices" >> test_result_e3.txt
node app.js --addRest "Blue Lobster2" "Low Low prices" >> test_result_e3.txt

echo ----ADD RESERVATIONS---- >> test_result_e3.txt
node app.js --addResv  "Red Lobster" "Mar 17 2019 15:15:00" 5 >> test_result_e3.txt
node app.js --addResv  "Blue Lobster" "Mar 17 2019 17:15:00" 5 >> test_result_e3.txt
node app.js --addResv  "Blue Lobster2" "Mar 17 2019 17:16:00" 5 >> test_result_e3.txt
node app.js --addResv  "Blue Lobster" "Mar 17 2019 17:16:00" 5 >> test_result_e3.txt
node app.js --addResv  "Blue Lobster" "Mar 18 2019 17:16:00" 5 >> test_result_e3.txt
node app.js --addResv  "Blue Lobster" "Mar 16 2019 17:10:00" 5 >> test_result_e3.txt
node app.js --addResv  "Blue Lobster" "Mar 17 2019 16:19:00" 5 >> test_result_e3.txt
node app.js --addResv  "Blue Lobster" "Mar 17 2019 21:16:58" 5 >> test_result_e3.txt

echo ----ALL RESERVATIONS---- >> test_result_e3.txt
node app.js --allResv "Blue Lobster" >> test_result_e3.txt

echo ----ALL RETAURANTS---- >> test_result_e3.txt
node app.js --allRest >> test_result_e3.txt

echo ----RETAURANTS INFO---- >> test_result_e3.txt
node app.js --restInfo "Red Lobster" >> test_result_e3.txt
echo -------- >> test_result_e3.txt
node app.js --restInfo "Blue Lobster" >> test_result_e3.txt
echo ----HOUR RESV---- >> test_result_e3.txt
node app.js --hourResv "Mar 17 2019 16:30:00" >> test_result_e3.txt
echo ----ADD DELAY---- >> test_result_e3.txt
node app.js --addDelay "Blue Lobster2" 120 >> test_result_e3.txt
echo -------- >> test_result_e3.txt
node app.js --addDelay "Blue Lobster" 220 >> test_result_e3.txt

echo ----CHECK OFF---- >> test_result_e3.txt
node app.js --checkOff "Blue Lobster" >> test_result_e3.txt
node app.js --checkOff "Blue Lobster2" >> test_result_e3.txt
echo ----CHECK---- >> test_result_e3.txt
node app.js --allResv "Blue Lobster" >> test_result_e3.txt
node app.js --allResv "Blue Lobster2" >> test_result_e3.txt
node app.js --allRest >> test_result_e3.txt

echo ----STATUS---- >> test_result_e3.txt
node app.js --status >> test_result_e3.txt

del /f reservations.json

del /f restaurants.json

del /f status.json

pause >nul