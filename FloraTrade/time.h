//to find transaction time
long long get_time(chrono::steady_clock::time_point* programStartTime){
    auto currentTime = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - *programStartTime).count();
    //return "000";
}

// Function to initialize and return the initial time components
std::tm getInitialTimeInfo() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm time_info = *std::localtime(&now_c);
    return time_info;
}

// Function to get the updated time string based on elapsed time
std::string getUpdatedTimeString(long long elapsedMilliseconds, std::tm time_info ) {
   
    // Update time components based on elapsed time
    time_info.tm_sec = time_info.tm_sec + elapsedMilliseconds / 1000;
    mktime(&time_info);

    // Calculate the updated milliseconds
    int updatedMilliseconds = elapsedMilliseconds % 1000;

    // Format the time as a string
    std::stringstream ss;
    ss << std::put_time(&time_info, "%Y%m%d-%H%M%S") << "." << std::setw(3) << std::setfill('0') << updatedMilliseconds;

    return ss.str();
}