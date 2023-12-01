#include <chrono> // для функций из std::chrono

using namespace std;
using namespace chrono;

class Timer
{
private:
	// Псевдонимы типов используются для удобного доступа к вложенным типам
//	using clock_t = std::chrono::high_resolution_clock;
//	using second_t = std::chrono::duration<double, std::ratio<1> >;


	using second_t = duration<double, std::ratio<1> >;
	time_point<steady_clock> begin;
	time_point<steady_clock> end;
 
public:
	Timer() // : m_beg(clock_t::now())
	{
	}
 
	void start(const char* txt="Timer started... \n")
	{
		cout << txt;
		begin = steady_clock::now();
	}

	void stop(const char* txt="Timer stopped... \n")
	{
		cout << txt;
		end = steady_clock::now();
	}

	double elapsed_sec() const
	{
		auto sec = duration_cast<second_t>(end - begin);
		return sec.count();
	}
	
	int64_t elapsed_ms() const
	{
		auto sec = duration_cast<milliseconds>(end - begin);
		return sec.count();
	}
	int64_t elapsed_usec() const
	{
		auto sec = duration_cast<microseconds>(end - begin);
		return sec.count();
	}

	void spent_us(const char* txt="Time spent (microseconds): ")
	{	
		  cout << txt << (elapsed_sec()*1000000) << '\n';
	}
	

	void spent_msec(const char* txt="Time spent (milliseconds): ")
	{	
		  cout << txt << setprecision(3) << (elapsed_sec()*1000) << '\n';
	}

	void spent_sec(const char* txt="Time spent (seconds): ")
	{	
		  cout << txt << setprecision(3) << (elapsed_sec()) << '\n';
	}

};

Timer t;

