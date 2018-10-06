#include <chrono>
#include <string>
#include <iostream>

class ProgressBar
{
public:
  ProgressBar():
    initialized_(false),
    bar_id_(0)
  {}

  ProgressBar(int total, int barwidth) :
    initialized_(false),
    barwidth_(barwidth),
    bar_id_(0)
  {}
  
  ~ProgressBar()
  {
//    std::cout << std::endl;
  }
  
  void init(int total, int barwidth)
  {
    initialized_ = false;
    barwidth_ = barwidth;
    total_ = total;
    last_completed_ = 0;
    set_theme_random();
  }

  int set_theme_random()
  {
     srand (time(NULL));
     bar_id_ = rand() % bars_.size();
  }

  void set_theme_solid_bar() {bar_id_ = 0;}
  void set_theme_line() { bar_id_ = 1; }
  void set_theme_circle() { bar_id_ = 2; }
  void set_theme_braille() { bar_id_ = 3; }
  void set_theme_braille_spin() { bar_id_ = 4; }
  
  void print(int completed)
  {
    if (!initialized_)
    {
      last_print_time_ = std::chrono::system_clock::now();
      start_time_ = std::chrono::system_clock::now();
      initialized_ = true;
    }
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    // limit printing to about 30 Hz
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_print_time_).count() > 33
        || completed == total_)
    {
      double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time_).count() / 1000.0;
      last_print_time_ = now;
      std::cout << " \r [";
      double pos = barwidth_ * (completed / (double)total_);
      for (int i = 0; i < barwidth_; ++i)
        if (i < floor(pos)) std::cout << *(bars_[bar_id_].end()-1);
        else if (i == floor(pos)) std::cout << bars_[bar_id_][round((pos - floor(pos)) * (bars_[bar_id_].size() -1))];
        else std::cout << bars_[bar_id_][0];
      std::cout << "]  ";
      printf("%.0f%% ", (completed / (double)total_)*100.0);
      double it_s = completed / elapsed;
      std::string left_stamp = ms_to_stamp(((total_ - completed) / it_s)*1000);
      std::string elapsed_stamp = ms_to_stamp(elapsed * 1000.0);
      printf("[%s<%s, %.2fit/s] ", elapsed_stamp.c_str(), left_stamp.c_str(), it_s);
      std::cout.flush();
    }
    last_completed_ = completed;
  }
private:

  std::string ms_to_stamp(int ms)
  {
    if (ms <= 0.0)
    {
      return "";
    }
    int millis = ms % 1000;
    int sec = ((ms - millis) % (60 * 1000)) / 1000;
    int min = ((ms - (millis + sec*1000)) % (60 * 60 * 1000)) / (60*1000);
    int hour = ((ms - (millis + (sec + min*60)*1000)) % (24 * 60 * 60 * 1000)) / (60*60*1000);
    int day = ((ms - (millis + (sec + (min + hour * 60) * 60) * 1000)) / (24 * 60 * 60 * 1000))/(24*60*60*1000);
    char buf[25];
    int n;
    if (day > 0)
      n = sprintf(buf, "%d:%d:%02d:%02d:%03d", day, hour, min, sec, millis);
    else if (hour > 0)
      n = sprintf(buf, "%d:%02d:%02d:%03d", hour, min, sec, millis);
    else if (min > 0)
      n = sprintf(buf, "%d:%02d:%03d", min, sec, millis);
    else if (sec > 0)
      n = sprintf(buf, "%d:%03d", sec, millis);
    else
      n = sprintf(buf, "%d", millis);
    std::string out(buf);
    return out;
  }



//  }

  int barwidth_;
  int total_;
  bool initialized_;
  int last_completed_;
  int bar_id_;
  std::vector< std::vector< const char*> > bars_{
    {" ", "▏", "▎", "▍", "▋", "▋", "▊", "▉", "▉", "█"},
    {"─", "─", "─", "╾", "╾", "╾", "╾", "━", "═"},
    {" ", "◓", "◑", "◒", "◐", "◓", "◑", "◒", "#"},
    {" ", ".", "o", "O", "0", "O", "o", ".", "█"},
    {" ", "⡀", "⡄", "⡆", "⡇", "⡏", "⡟", "⡿", "⣿"},
    {" ", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠇", "⠿"}};

  
  std::chrono::system_clock::time_point start_time_;
  std::chrono::system_clock::time_point last_print_time_;  
};