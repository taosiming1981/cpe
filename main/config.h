#pragma once
#include <string>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cstring>
#include <libconfig.h++>

using namespace std;
using namespace libconfig;

struct ServerInfo {
    std::string ip;
    int  port;
};


struct TunnelInfo {
    std::string local_ip;
    std::string server_ip;
    std::string server_port;
    std::string up_bw; //Mbps
    std::string down_bw;
    std::string priority;
};

struct NodeInfo {
    unsigned int nodeID;
    std::string virtualIP;
    std::string subNet;
    unsigned int netMask;
    std::string macAddr;
};

class mzConfig
{
public:
    mzConfig()
    {
        confPath = "";
        reset();
    }

    ~mzConfig(){}

public:
    inline void reset(){
        userID = 0;
        connectMode = 0;
        timerInterval = 0;
        packetSize = 0;
        gateWay = 0;
        meshBw = 0;
        gatewayBw = 0;
        netMask = 24;
        debug  = 0;
        thread = 0;
        sleepMs = 10;
        tapMode = 0;
        bondMode = 0;
        statistics = 0;
        onlyAcc = 0;

        userIP = "";
        devName = "";
        upCmd = "";
        downCmd = "";
        logFile = "";
        encrypt = "";
        logFile = "./app.log";
        statFile = "./stat.log";
        meshFile = "./mesh.txt";
    }

    inline void reload() {
        if(confPath.size() == 0)
           return;

        reset();
        loadConfig(confPath.c_str());
    }

    inline static vector<string> split(const string &str,const string &pattern)
    {
        //const char* convert to char*
        char * strc = new char[strlen(str.c_str())+1];
        strcpy(strc, str.c_str());
        vector<string> resultVec;
        char* tmpStr = strtok(strc, pattern.c_str());
        while (tmpStr != NULL)
        {
            resultVec.push_back(string(tmpStr));
            tmpStr = strtok(NULL, pattern.c_str());
        }

        delete[] strc;

        return resultVec;
    }

    inline int setMacAddr(std::string mac)
    {
         Config cfg;
         try    {
            cfg.readFile(confPath);
         } catch(const FileIOException &fioex)  {
            std::cerr << "I/O error while reading file." << std::endl;
            return(EXIT_FAILURE);
         } catch(const ParseException &pex)  {
             std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                 << " - " << pex.getError() << std::endl;
             return(EXIT_FAILURE);
         }

        Setting& root = cfg.getRoot();
        if(root.exists("macAddr")){
           root.remove("macAddr");
        }
           root.add("macAddr", libconfig::Setting::TypeString)=mac; 
          

        cout << "config file mac Addr:" << macAddr << endl;
        try {
            std::string output_file = confPath;
            cfg.writeFile(output_file);
            std::cout << "Updated configuration successfully written to: " << output_file << std::endl;
        } catch(const FileIOException &fioex) {
            std::cerr << "I/O error while writing file: "  << std::endl;
            return(EXIT_FAILURE);
        }
       return 0;
    }

    inline int loadConfig(const char* fileName){
        //fileName = "mingzhui.cfg";
        if(confPath.size() == 0)
            confPath = fileName;

        Config cfg;
        try    {
            cfg.readFile(fileName);
        } catch(const FileIOException &fioex)  {
            std::cerr << "I/O error while reading file:" << fileName << std::endl;
            return(EXIT_FAILURE);
        } catch(const ParseException &pex)  {
             std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                 << " - " << pex.getError() << std::endl;
             return(EXIT_FAILURE);
        }


        try  {
            userID = cfg.lookup("userID");
            cout << "userID: " << userID <<  endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'userID' setting in configuration file." << endl;
        }

        try  {
            gateWay = cfg.lookup("gateWay");
            cout << "gateWay: " << gateWay <<  endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'gateWay' setting in configuration file." << endl;
        }

        try  {
            tapMode = cfg.lookup("tapMode");
            cout << "tapMode: " << tapMode <<  endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'tapMode' setting in configuration file." << endl;
        }

        try  {
            onlyAcc = cfg.lookup("onlyAcc");
            cout << "onlyAcc: " << onlyAcc <<  endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'onlyAcc' setting in configuration file." << endl;
        }

        try  {
            bondMode = cfg.lookup("bondMode");
            cout << "bondMode: " << bondMode <<  endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'bondMode' setting in configuration file." << endl;
        }


        try  {
            statistics = cfg.lookup("statistics");
            cout << "statistics " << statistics <<  endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'statistics' setting in configuration file." << endl;
        }

        try  {
            meshBw = cfg.lookup("meshBw");
            cout << "meshBw: " << meshBw <<  endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'meshBw' setting in configuration file." << endl;
        }

        try{
            connectMode = cfg.lookup("connectMode");
            cout << "connect mode:" << connectMode << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'connectMode' setting in configuration file." << endl;
        }

        try{
            timerInterval = cfg.lookup("timerInterval");
            cout << "timer interval:" << timerInterval << endl; 
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'timerInterval' setting in configuration file." << endl;
        }

        try{
            sleepMs = cfg.lookup("sleepMs");
            cout << "sleepMs:" << sleepMs  << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'sleepMs' setting in configuration file." << endl;
        }
 
        try{
            gatewayBw = cfg.lookup("gatewayBw");
            cout << "gatewayBw:" << gatewayBw << endl;            
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'gatewayBw' setting in configuration file." << endl;
        }

        try{
            cfg.lookupValue("devName", devName);
            cout << "tun dev device name:" << devName << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'devName' setting in configuration file." << endl;
        }

        try{
            cfg.lookupValue("macAddr", macAddr);
            cout << "tun dev device name:" << macAddr << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'macAddr' setting in configuration file." << endl;
        }

        try{
            cfg.lookupValue("venderID", venderID);
            cout << "venderID name:" << venderID << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'venderID' setting in configuration file." << endl;
        }


        try{
            cfg.lookupValue("userIP", userIP);
            cout << "userIP name:" << userIP << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'userIP' setting in configuration file." << endl;
        }

        try{
            netMask = cfg.lookup("netMask");
            cout << "netmask name:" << netMask << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'netMask' setting in configuration file." << endl;
        }

        try{
            cfg.lookupValue("outDevNameMaster", out_dev_name_master);
            cout << "outDevNameMaster name:" << out_dev_name_master << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'outDevNameMaster' setting in configuration file." << endl;
        }

        try{
            cfg.lookupValue("outDevNameSlave", out_dev_name_slave);
            cout << "outDevNameSlave name:" << out_dev_name_slave << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'outDevNameSlave' setting in configuration file." << endl;
        }

        try{
            cfg.lookupValue("upCmd", upCmd);
            cout << "upCmd:" << upCmd << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'upCmd' setting in configuration file." << endl;
        }


        try{
            cfg.lookupValue("downCmd", downCmd);
            cout << "downCmd name:" << downCmd << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'dowmCmd' setting in configuration file." << endl;
        }


        try{
            cfg.lookupValue("logFile", logFile);
            cout << "logFile name:" << logFile << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'logFile' setting in configuration file." << endl;
        }

       try{
            cfg.lookupValue("statFile", statFile);
            cout << "statFile name:" << statFile << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'statFile' setting in configuration file." << endl;
        }

       try{
            cfg.lookupValue("meshFile", meshFile);
            cout << "meshFile name:" << meshFile << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'meshFile' setting in configuration file." << endl;
        }


       try{
            cfg.lookupValue("encrypt", encrypt);
            cout << "encrypt name:" << encrypt << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'encrypt' setting in configuration file." << endl;
        }

        try{
            cfg.lookupValue("debug", debug);
            cout << "debug name:" << debug << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'debug' setting in configuration file." << endl;
        }

        try{
            cfg.lookupValue("thread", thread);
            cout << "thread name:" << thread << endl;
        } catch(const SettingNotFoundException &nfex)  {
            cerr << "No 'thread' setting in configuration file." << endl;
        }


        const Setting& root = cfg.getRoot();
#if 0
        try{
            const Setting& edgeSvrs = root["edgeSvr"];
            for(int n = 0; n < edgeSvrs.getLength(); n++){
                 std::string edge_info = edgeSvrs[n];
                 if(edge_info.size() == 0)
                     continue;
            
                 vector<string> dataVec = split((const string&)edge_info, ":");
                 if(dataVec.size() != 2)
                     continue;
    
                 struct ServerInfo info;
                 info.ip = dataVec[0];
                 info.port = atoi(dataVec[1].c_str());
                 serverList.push_back(info); 
                 cout << "edge server ip:" << edge_info << endl;
            }
        
        }catch(const SettingNotFoundException &nfex){
           cerr << "No 'edgeSvr' setting in configuration file." << endl;
        }
#endif
#if 1
        try
        {
            const Setting &tunnels = root["tunnel_list"];
            int   tunnel_count = tunnels.getLength();
            for(int k = 0; k < tunnel_count; ++k){
                const Setting &tunnel = tunnels[k];
                struct TunnelInfo info;
                if(!(tunnel.lookupValue("local_ip", info.local_ip)
                   && tunnel.lookupValue("server_ip", info.server_ip)
                   && tunnel.lookupValue("server_port", info.server_port)
                   && tunnel.lookupValue("up_bandwidth", info.up_bw)
                   && tunnel.lookupValue("down_bandwidth", info.down_bw)
                   && tunnel.lookupValue("priority", info.priority)
                ))
                continue;

               cout << setw(10) << left << info.local_ip    << "  "
                    << setw(30) << left << info.server_ip   << "  "
                    << setw(30) << left << info.server_port << "  "
                    << setw(6)  << left << info.up_bw       << "  "
                    << setw(6)  << left << info.down_bw     << "  "
                    << setw(6)  << right<< info.priority    << endl;

               tunnelList.push_back(info);
            }
        }
        catch(const SettingNotFoundException &nfex)
        {
        }
#endif
        return 0;
    }

public:
    unsigned int userID;
    unsigned int connectMode;
    unsigned int timerInterval;
    unsigned int packetSize;
    unsigned int gatewayBw;
    unsigned int gateWay;
    unsigned int meshBw;
    unsigned int debug;
    unsigned int thread;
    unsigned int netMask;
    unsigned int sleepMs;
    unsigned int tapMode;
    unsigned int bondMode;
    unsigned int statistics;
    unsigned int onlyAcc;

    std::string  venderID;
    std::string  userIP;
    std::string  devName;
    std::string  logFile;
    std::string  statFile;
    std::string  meshFile;
    std::string  upCmd;
    std::string  downCmd;
    std::string  confPath;
    std::string  macAddr;
    std::string  encrypt;
    std::string  out_dev_name_master;
    std::string  out_dev_name_slave;
    std::vector<TunnelInfo> tunnelList;
    std::vector<ServerInfo> serverList;
    std::vector<NodeInfo> nodeInfoList;
};

