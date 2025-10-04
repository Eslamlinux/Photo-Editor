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

