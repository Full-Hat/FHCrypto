#include <curl/curl.h>
#include <iostream>

int download(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <URL> <LocalFilePath>" << std::endl;
        return -1;
    }

    std::string url = argv[1];
    std::string localFilePath = argv[2];

    CURL* curl = curl_easy_init();
    if(curl) {
        FILE* fp = fopen(localFilePath.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        CURLcode res = curl_easy_perform(curl);
        fclose(fp);
        curl_easy_cleanup(curl);

        if(res != CURLE_OK) {
            std::cerr << "Failed to download file." << std::endl;
            return -1;
        }
    }

    std::cout << "File successfully downloaded." << std::endl;
    return 0;
}
