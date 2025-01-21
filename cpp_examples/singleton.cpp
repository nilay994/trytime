
#include <iostream>
#include <string>

namespace uvf::lars_sh_controller
{
/**
 * A singleton to provide topic names to the uvm_lars_sh_coontroller package
 */
class GetRovTopics
{

 public:
    /**
     *  Singleton should not be cloneable and assignable
     */
    GetRovTopics(const GetRovTopics&) = delete;
    GetRovTopics& operator=(const GetRovTopics&) = delete;
    GetRovTopics(GetRovTopics&&)                 = delete;
    GetRovTopics& operator=(GetRovTopics&&) = delete;

    /**
     * Gets the UiHeartbeatCommand topic name to use
     * @return The topic name as a string
     */
    static std::string GetUiHeartbeatCommandTopic()
    {
        GetRovTopics& instance = GetRovTopics::GetInstance();
        return std::string("here..\n");
    }

    /**
     * Gets the UiControlUnavailableStatus topic name to use
     * @return The topic name as a string
     */
    static std::string GetUiControlUnavailableStatusTopic();

 private:
    std::string m_UiHeartbeatCommandTopic;         // "/fugro/rov/ui/lars_sh/heartbeat/command";
    std::string m_UiControlUnavailableStatusTopic; // "/fugro/rov/ui/lars_sh/control_unavailable/status";

    GetRovTopics() { std::cout << "private constructor called\n" << std::endl; }
    /**
     * A getter for the static singleton
     * @return a reference to the singleton instance
     */
    static GetRovTopics& GetInstance()
    {
        static GetRovTopics instance;
        return instance;
    }
};

} // namespace uvf::lars_sh_controller

int main()
{
    std::string ret_val = uvf::lars_sh_controller::GetRovTopics::GetUiHeartbeatCommandTopic();
    std::cout << "out: " << ret_val << std::endl;
    return 0;
}