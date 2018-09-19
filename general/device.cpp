#include "device.hpp"
#include "app/app.hpp"

using namespace std;
namespace General{
namespace Command {
const char newline = '\r';
const string space = " ";
const string select = "sel";
const string feature = "ft";
const string echo = "echo";
const string duty = "dt";
const string motor_control = "mc";
const string async_motor_control = "rady";
const string sync = "go";
const string angle = "sc";
const string reset = "rst";
const string stop = "stop";
const string open = "opn";
const string close = "cls";
}  // namespace Command

namespace FeatureKeyword {
  const devicename_t motor = "moto";
  const devicename_t solenoid = "sole";
}

DeviceManager::DeviceManager(string filename, int rate) : serial(io) {
    try {
        serial.open(filename);
    } catch (...) {
        app->report->Error(Util::ReportGroup::SerialPort,
                           "Failed to Open SerialPort");
    }
    cout << "Serialport opened successfully" << endl;
    serial.set_option(
        boost::asio::serial_port_base::baud_rate(rate));  //ボーレート設
    cout << "Baud rate set to " << rate << endl;
    //WriteSerial("foo");
}

DeviceManager::~DeviceManager() {
    serial.close();
    cout << "Serialport closed" << endl;
}

/// デバイスのインスタンスを生成

/*
bool DeviceManager::CreateMotor(address_t new_address) {
    //アドレスが本当に新しいか確認
<<<<<<< HEAD
    if(devices_address.count(new_address) == 0){
      //新しいアドレスでMotorへのweakポインタを生成する
      shared_ptr<DeviceMotor> new_sptr =
          make_shared<DeviceMotor>(this, new_address);
      devices_address[new_address] = new_sptr;
      return true;
      //return ;
    }
    else{//そのアドレスは既に埋まっているならば
      //TODO エラーレポート　address XX is already taken
      //return
devices_address[new_address];//devices_addressの中のそのアドレスの要素を返す
      return false;
    }

=======
    if (devices_adr.count(new_address) == 0) {
        //新しいアドレスでMotorへのweakポインタを生成する
        shared_ptr<DeviceMotor> new_sptr =
            make_shared<DeviceMotor>(this, new_address);
        devices_adr[new_address] = new_sptr;
        return true;
        // return ;
    } else {  //そのアドレスは既に埋まっているならば
        // TODO エラーレポート　address XX is already taken
        // return
        // devices_adr[new_address];//devices_adrの中のそのアドレスの要素を返す
        return false;
    }
>>>>>>> development
}
*/

///　デバイスのインスタンスを生成（終）

void DeviceManager::WriteSerial(
    const string& text) {  //渡された文字に'\r'をつけて送信
    this_thread::sleep_for(chrono::milliseconds(10));
    cout << "WriteSerial: " <<text << endl;
    boost::asio::write(serial, boost::asio::buffer(text + Command::newline));
}

optional<string>
DeviceManager::ReadSerial() {  //必ず'\r'で終わる文字列を一つ読み込む
    debug("ReadSerial called");
    boost::asio::streambuf buf;
    future<void> timed_task = async(launch::async, [&] {
        boost::asio::read_until(serial, buf, Command::newline);
    });
    future_status timer = timed_task.wait_for(SERIAL_TIME_LIMIT);
    if (timer != future_status::timeout) {
        string result = boost::asio::buffer_cast<const char*>(
            buf.data());  //バッファの中身を文字列として取り出す
        if (!result.empty())  //空ではありえないはずだが、念の為
            result.pop_back();  //末尾'\r'を削除
        cout << "ReadSerial: " <<result << endl;
        debug("ReadSerial returning");
        return result;
    } else {
        // error serial read timeout
        cout << "ReadSerial: TIMEOUT" << endl;
        return nullopt;
    }
}

void DeviceManager::CacheAddress(address_t adr,
                                 std::weak_ptr<DeviceBase> wptr) {
    devices_address.emplace(adr, wptr);
}

void DeviceManager::CacheFeature(address_t adr, factor_t fac) {
    devices_feature.emplace(fac, devices_address[adr]);
}

void DeviceManager::CacheFeature(factor_t fac, std::weak_ptr<DeviceBase> wptr) {
    devices_feature.emplace(fac, wptr);
}

void DeviceManager::Select(address_t address) {
    stringstream ss;
    ss << Command::select << Command::space << address;  //例:ss="sel 39"
    WriteSerial(ss.str());
}

// void DeviceManager::PushCommandDirectly(function<void()> no_sel){
// command.push(no_sel);
//}

int DeviceManager::Fetch() {
    queue<function<void()>> send;
    queue<function<void()>> receive;
    int count = 0;
    for (auto& dev : devices_address) {
        cout << "a device" << endl;
        if (shared_ptr<DeviceBase> sptr = dev.second.lock()) {
            if (!(sptr->async_task.empty())) {
                send.push([=] { Select(dev.first); });
                receive.push([=] {
                  //NO RESPONSE
                  /*
                    if (ReadSerial() == "GOOD RESPONSE") {
                        ////
                    } else {
                        ////
                    }
                  */
                });
                do {
                    string to_send = get<string>(sptr->async_task.front());
                    send.push([=] {
                        WriteSerial(to_send);
                    });
                    auto response_checker = get<function<void(optional<string>)>>(
                        sptr->async_task.front());
                    receive.push([=] {
                        response_checker(ReadSerial());
                    });
                    sptr->async_task.pop();
                    count++;
                } while (!(sptr->async_task.empty()));
            }
        }
    }
    while (!send.empty()) {
      cout << "send command push" << endl;
        command.push(send.front());
        send.pop();
    }
    while (!receive.empty()) {
      cout << "receive command push" << endl;
        command.push(receive.front());
        receive.pop();
    }
    return count;
}

void DeviceManager::Flush(future<int>& task) {
    cout << "Flush!" << endl;
    cout << command.size() << endl;
    task = async(launch::async, [&] {
        int count = 0;
        while (!command.empty()) {
            debug("flush1");
            auto next = command.front();
            command.pop();
            next();

            debug("flush2");
            count++;
        }
        return count;
    });
    /*
    while (!command.empty()) {
        command.pop();
    }
    */
    /*
     = async(launch::async,
      [=]{
        while(!command.empty()){

        }
      }
    );
    */
}

vector<shared_ptr<DeviceBase>> DeviceManager::SearchFeature(factor_t target) {
    vector<shared_ptr<DeviceBase>> result;
    auto search = devices_feature.equal_range(target);
    for (auto it = search.first; it != search.second; ++it) {
        shared_ptr<DeviceBase> sptr = (it->second).lock();
        result.push_back(sptr);
    }
    return result;
}
/*
vector<shared_ptr<DeviceBase>> DeviceManager::SearchFeature(factor_t target){

}
*/

std::vector<std::shared_ptr<DeviceBase>> DeviceManager::AllDevices() {
    vector<shared_ptr<DeviceBase>> all;
    for (auto it = devices_feature.cbegin(); it != devices_feature.cend();
         it++) {
        shared_ptr<DeviceBase> sptr = (it->second).lock();
        all.push_back(sptr);
    }
    return all;
}

// template <typename T>
// std::vector<T> VectorAnd(std::vector<T> vec1, std::vector<T> vec2){
//  return vec1;
//}

DeviceBase::DeviceBase() {  //デバイスのインスタンスを生成時、sel XX と ftを送る
  /*
    PushCommand(Command::feature, [=](optional<string> response) {
        if (Feature(response)) {
            cout << "Feature success" << endl;
        } else {
            /// Statements
        }
    });
  */
}

DeviceBase::~DeviceBase() {
    // DeviceManagerが持つこのDeviceBaseへのweakポインタはここで無効になる
}

void DeviceBase::PushCommand(
    string to_send,
    function<void(optional<string>)> response_checker) {
    async_task.push(make_tuple(to_send, response_checker));
}

void DeviceBase::ReadCSV(string str) {
    feature.clear();
    auto first = str.begin();
    while (first != str.end()) {
        auto last = first;
        while (last != str.end() && *last != ',') {
            ++last;
        }
        feature.emplace(string(first, last));
        parent->CacheFeature(address, string(first, last));
        if (last != str.end()) {
            ++last;
        }
        first = last;
    }
}

bool DeviceBase::Feature(optional<string> response) {  //
    if (response) {
        ReadCSV(*response);
        cout << "Feature receive successfully from Device" << address << endl;
        return true;
    } else {
        cout << "Failed to get Feature from Device" << address << endl;
        cout << "(No Response)" << endl;
        return false;
    }
}

void DeviceBase::Echo(string str) {
    PushCommand(Command::echo+Command::space+str, [=](optional<string> response) {
      /*
        if (response) {
            if (*response == str) {
                // cout << "Device " << address << "is responding properly." <<
                // endl;
            } else {
                // cout << "Device " << address << "is NOT responding properly."
                // << endl;
            }
        } else {
            //cout << "Device " << address << " is NOT responding!" << endl;
        }
      */
    });
}

void DeviceBase::Reset() {
    PushCommand(Command::reset, [=](optional<string> response) {
        if (response == "GOOD RESPONSE") {
            /// Statements
        } else {
            /// Statements
        }
    });
}

DeviceMotor::DeviceMotor(std::shared_ptr<DeviceManager> p, address_t a) {
    parent = p;
    address = a;
}

DeviceMotor::~DeviceMotor() {}

void DeviceMotor::
    Synchronize() {  // selを伴わない、
                     // 同時に実行させたい範囲の開始を示す特殊な命令
    /*
      parent->Fetch();
      parent->PushCommandDirectly(
        [=]{
          parent->WriteSerial(Command::sync + Command::newline);
        }
      );
      parent->PushCommandDirectly(
        [=]{
          if (parent->ReadSerial() == "GOOD RESPONSE") {
              /// Statements
          } else {
              /// Statements
          }
        }
      );
      // parent->WriteSerial(ss.str());
    */
}

void DeviceMotor::Duty(float value) {
    stringstream ss;
    ss << Command::duty << Command::space << value;
    PushCommand(ss.str(), [=](optional<string> response) {
        if (response == "GOOD RESPONSE") {
            /// Statements
        } else {
            /// Statements
        }
    });
}

DeviceSolenoid::DeviceSolenoid(std::shared_ptr<DeviceManager> p, address_t a) {
    parent = p;
    address = a;
}

DeviceSolenoid::~DeviceSolenoid() {}

void DeviceSolenoid::Open(int id) {
    stringstream ss;
    ss << Command::open << Command::space << id;
    PushCommand(ss.str(), [=](optional<string> response) {
        if (response == "GOOD RESPONSE") {
            /// Statements
        } else {
            /// Statements
        }
    });
}

void DeviceSolenoid::Close(int id) {
    stringstream ss;
    ss << Command::close << Command::space << id;
    PushCommand(ss.str(), [=](optional<string> response) {
        if (response == "GOOD RESPONSE") {
            /// Statements
        } else {
            /// Statements
        }
    });
}

void DeviceSolenoid::Open() {
    Open(0);
}

void DeviceSolenoid::Close() {
    Close(0);
}
}
