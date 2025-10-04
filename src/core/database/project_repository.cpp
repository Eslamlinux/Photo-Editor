#include "project_repository.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>

namespace pme {
namespace core {
namespace database {

// تنفيذ خاص للمستودع
class ProjectRepository::Impl {
public:
    Impl() : m_initialized(false) {}
    
    bool initialize() {
        // في هذا التنفيذ البسيط، سنستخدم ملف نصي لتخزين المشاريع
        m_dbPath = "projects.txt";
        m_initialized = true;
        
        // التحقق من وجود الملف
        std::ifstream file(m_dbPath);
        if (!file.good()) {
            // إنشاء ملف جديد
            std::ofstream newFile(m_dbPath);
            if (!newFile.good()) {
                std::cerr << "Failed to create database file: " << m_dbPath << std::endl;
                m_initialized = false;
                return false;
            }
            newFile.close();
        }
        file.close();
        
        return m_initialized;
    }

} // namespace pme
   bool createProject(const std::string& name, const std::string& path, const std::string& thumbnail_path) {
        if (!m_initialized) {
            return false;
        }
        
        // قراءة المشاريع الحالية
        std::vector<Project> projects = getAllProjects();
        
        // إنشاء معرف جديد
        int newId = 1;
        if (!projects.empty()) {
            newId = projects.back().id + 1;
        }
        
        // الحصول على الوقت الحالي
        std::time_t now = std::time(nullptr);
        char timeStr[20];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        
        // إضافة المشروع الجديد
        std::ofstream file(m_dbPath, std::ios::app);
        if (!file.good()) {
            std::cerr << "Failed to open database file for writing: " << m_dbPath << std::endl;
            return false;
        }
        
        file << newId << "|" << name << "|" << path << "|" << thumbnail_path << "|" << timeStr << "|" << timeStr << std::endl;
        file.close();
        
        return true;
    }
    
    std::optional<Project> getProjectById(int id) {
        if (!m_initialized) {
            return std::nullopt;
        }
        
        // قراءة المشاريع

