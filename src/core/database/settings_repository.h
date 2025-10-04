#ifndef PME_CORE_DATABASE_SETTINGS_REPOSITORY_H
#define PME_CORE_DATABASE_SETTINGS_REPOSITORY_H

#include <string>
#include <map>
#include <memory>

namespace pme {
namespace core {
namespace database {

class SettingsRepository {
public:
    SettingsRepository();
    ~SettingsRepository();

    // تهيئة المستودع
    bool initialize();
    
    // الحصول على قيمة إعداد
    std::string getSetting(const std::string& key, const std::string& defaultValue = "");
    
    // تعيين قيمة إعداد
    bool setSetting(const std::string& key, const std::string& value);
    
    // حذف إعداد
    bool deleteSetting(const std::string& key);
    
    // الحصول على جميع الإعدادات
    std::map<std::string, std::string> getAllSettings();

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace database
} // namespace core
} // namespace pme

#endif // PME_CORE_DATABASE_SETTINGS_REPOSITORY_H
