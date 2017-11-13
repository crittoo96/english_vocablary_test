#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <sstream>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

inline double get_time_sec(void){
    return static_cast<double>(duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count())/1000000000;
}

// Question作成時にeng.txtを読み込む、
// 
class Question {
private:
    // buffer
    vector<vector<string> > vocabularies;
    int ans_index;  // define constructor, random within file_lines
    int dammy_index;    // is this necessary?
    char mode;  // 0 is eng to jap, 1 is jap to eng
    int line_count;
    int choices;
    double ans_speed_bound;
    
public:
    Question(){
        // load eng.txt
        ifstream reading_file;
        reading_file.open("eng.txt");
        string reading_file_line_buffer;

        line_count = 0;
        while(!reading_file.eof()){
            getline(reading_file, reading_file_line_buffer);
            // cout << reading_file_line_buffer << endl;

            const char delimiter = ',';
            string separated_string_buffer;
            istringstream line_separater(reading_file_line_buffer);
            int insert_c = 0;
            vector<string> temp(2, "");
            while(getline(line_separater, separated_string_buffer, delimiter)){
                insert_c++;
                if (insert_c % 2 == 0){
                    temp.push_back(separated_string_buffer);
                    temp[1] = separated_string_buffer;
                    vocabularies.push_back(temp);
                }
                else
                    temp[0] = separated_string_buffer;
            }
            line_count++;
        }

        mode = 1;
        choices = 4;
        ans_speed_bound = 5.0;
    }
    void AddLine(const string &line){
        //  vocabularies.push_back(line);
    }
    string GetAns(){
        return vocabularies[ans_index][mode];
    }
    void ShowAllData(){
        for(int i = 0; i < vocabularies.size(); i++){
            for(int j =0; j< 2; j++){
                cout << vocabularies[i][j] << endl;
            }
        }
    }
    int SetAns(){
        random_device rnd;
        mt19937 mt(rnd());
        uniform_int_distribution<> ans_generator(0, line_count - 1);

        return ans_generator(mt);    // index
    }
    void SelectMode(){
        char key;
        cout << "Selecting mode ..." << endl;
        cout << "input 0 is easy mode ... else is normal" << endl;
        cin >> key;
        if (key == '0')
            mode = 0;
        else
            mode = 1;
    }
    void ShowAnswers(const vector<int> &ans_indexes){
        for(auto x : ans_indexes){
            cout << x << " " << vocabularies[x][(mode == 0) ? 1 : 0] << endl;
        }
    }
    int Show(){
        int key = 0;
        int index = 0;
        ostringstream ss;
        vector<string> ans;
        vector<int> ans_indexes(choices);
        double start, stop;

        ans_index = SetAns();
        ss << ans_index;
        ans_indexes[0] = ans_index;
        ans.push_back(ss.str() + " " + vocabularies[ans_index][mode]);

        for(int i = 0; i < choices -1; i++){
            ostringstream oss;
            index = SetAns();
            oss << index;
            ans_indexes[i+1] = index;
            ans.push_back(oss.str() + " " + vocabularies[index][mode]);
        }
        // shuffle here
        random_shuffle(ans.begin(), ans.end());

        // show question
        (mode == 0) ? cout << vocabularies[ans_index][1] << endl : cout << vocabularies[ans_index][0] << endl;;

        for(int i = 0; i< choices; i++){
            cout << ans[i] << endl;
        }
        start = get_time_sec();
        cout << "Ans: "; cin >> key;
        
        if (0 <= key && key < line_count){
            if (key == ans_index){
                stop = get_time_sec();
                cout << "correct ";
                if (ans_speed_bound < stop - start){
                    cout << "but too late, so i think false" << endl;
                    ShowAnswers(ans_indexes);
                    return -1;
                }else{
                    cout << endl;
                    ShowAnswers(ans_indexes);
                    return 1;
                }
            }else {
                cout << "incorrect.. " << GetAns() << endl;
                ShowAnswers(ans_indexes);
                return -2;
            }
        }else
            return -2;
    }
};

class QuestionSheet {
    int question_numbers;
    int count;
public:
    QuestionSheet(const int &number){
        question_numbers = number;
        count = 0;
    }
    void Show(){
        Question *q = new Question();
        q->SelectMode();

        while(true){
            count = count + q->Show();
            if (count > question_numbers) break;
            ShowCount();
        }
    }
    void ShowCount(){
        cout << "---------------"  << count << endl;
    }
};

int main(){
    QuestionSheet *qs = new QuestionSheet(10);
    qs->Show();
}