#include "settings_repository.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace pme {
namespace core {
namespace database {

// تنفيذ خاص للمستودع
class SettingsRepository::Impl {
public:
    Impl() : m_initialized(false) {}
    
    bool initialize() {
        // في هذا التنفيذ البسيط، سنستخدم ملف نصي لتخزين الإعدادات
        m_dbPath = "settings.txt";
        m_initialized = true;
        
        // التحقق من وجود الملف
        std::ifstream file(m_dbPath);
        if (!file.good()) {
            // إنشاء ملف جديد
            std::ofstream newFile(m_dbPath);
            if (!newFile.good()) {
                std::cerr << "Failed to create settings file: " << m_dbPath << std::endl;
                m_initialized = false;
                return false;
            }
            newFile.close();
        } else {
            // قراءة الإعدادات الحالية
            loadSettings();
        }
        
        return m_initialized;
    }
    
    std::string getSetting(const std::string& key, const std::string& defaultValue) {
        if (!m_initialized) {
            return defaultValue;
        }
        
        auto it = m_settings.find(key);
        if (it != m_settings.end()) {
            return it->second;
        }
        
        return defaultValue;
    }
    
    bool setSetting(const std::string& key, const std::string& value) {
        if (!m_initialized) {
            return false;
        }
        
        // تحديث الإعداد في الذاكرة
        m_settings[key] = value;
        
        // حفظ الإعدادات في الملف
        return saveSettings();
    }
    
    bool deleteSetting(const std::string& key) {
        if (!m_initialized) {
            return false;
        }
        
        // حذف الإعداد من الذاكرة
        auto it = m_settings.find(key);
        if (it != m_settings.end()) {
            m_settings.erase(it);
            
            // حفظ الإعدادات في الملف
            return saveSettings();
        }
        
        return false;
    }
    
    std::map<std::string, std::string> getAllSettings() {
        return m_settings;
    }
    
private:
    bool m_initialized;
    std::string m_dbPath;
    std::map<std::string, std::string> m_settings;
    
    void loadSettings() {
        m_settings.clear();
        
        std::ifstream file(m_dbPath);
        if (!file.good()) {
            return;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) {
                continue;
            }
            
            size_t pos = line.find('=');
            if (pos == std::string::npos) {
                continue;
            }
            
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            m_settings[key] = value;
        }
        
        file.close();
    }
    
    bool saveSettings() {
        std::ofstream file(m_dbPath, std::ios::trunc);
        if (!file.good()) {
            std::cerr << "Failed to open settings file for writing: " << m_dbPath << std::endl;
            return false;
        }
        
        for (const auto& [key, value] : m_settings) {
            file << key << "=" << value << std::endl;
        }
        
        file.close();
        
        return true;
    }
};

// تنفيذ واجهة المستودع
SettingsRepository::SettingsRepository() : m_impl(std::make_unique<Impl>()) {
}

SettingsRepository::~SettingsRepository() = default;

bool SettingsRepository::initialize() {
    return m_impl->initialize();
}

std::string SettingsRepository::getSetting(const std::string& key, const std::string& defaultValue) {
    return m_impl->getSetting(key, defaultValue);
}

bool SettingsRepository::setSetting(const std::string& key, const std::string& value) {
    return m_impl->setSetting(key, value);
}

bool SettingsRepository::deleteSetting(const std::string& key) {
    return m_impl->deleteSetting(key);
}

std::map<std::string, std::string> SettingsRepository::getAllSettings() {
    return m_impl->getAllSettings();
}

} // namespace database
} // namespace core
} // namespace pme
