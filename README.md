gcc crawler.c
./crawler https://openai.com/blog/chatgpt crawler 3

crawler is the directory name (insert any name you want)
there was an error in curl.c and i couldnt run the program. therefore i included cacert.pem file in the folder and included lines 77, 78 in curl.c file.
	curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\Users\\PRABUDDHI NAVODYA\\Desktop\\test\\cacert.pem");
	curl_easy_setopt(curl, CURLOPT_CAPATH, "C:\\Users\\PRABUDDHI NAVODYA\\Desktop\\test\\cacert.pem");
if you run in windows using dev c++, include these 2 lines with your directory to the cacert.pem.  
also make sure to include libcurl4 in your machine and in dev cpp
