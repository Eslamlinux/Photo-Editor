#ifndef PME_CORE_DATABASE_PROJECT_REPOSITORY_H
#define PME_CORE_DATABASE_PROJECT_REPOSITORY_H

#include <string>
#include <vector>
#include <optional>
#include <memory>

namespace pme {
namespace core {
namespace database {

// نموذج مشروع
struct Project {
    int id;
    std::string name;
    std::string path;
    std::string thumbnail_path;
    std::string created_at;
    std::string updated_at;
};

class ProjectRepository {
public:
    ProjectRepository();
    ~ProjectRepository();

    // إنشاء مشروع جديد
    bool createProject(const std::string& name, const std::string& path, const std::string& thumbnail_path);
    
    // الحصول على مشروع بواسطة المعرف
    std::optional<Project> getProjectById(int id);
    
    // الحصول على جميع المشاريع
    std::vector<Project> getAllProjects();
    
    // تحديث مشروع
    bool updateProject(int id, const std::string& name, const std::string& path, const std::string& thumbnail_path);
    
    // حذف مشروع
    bool deleteProject(int id);
    
    // تهيئة قاعدة البيانات
    bool initialize();

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace database
} // namespace core
} // namespace pme

#endif // PME_CORE_DATABASE_PROJECT_REPOSITORY_H
