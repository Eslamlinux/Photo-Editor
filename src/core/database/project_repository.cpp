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

  std::vector<Project> projects = getAllProjects();
        
        // البحث عن المشروع
        for (const auto& project : projects) {
            if (project.id == id) {
                return project;
            }
        }
        
        return std::nullopt;
    }
    
    std::vector<Project> getAllProjects() {
        std::vector<Project> projects;
        
        if (!m_initialized) {
            return projects;
        }
        
        // قراءة المشاريع من الملف
        std::ifstream file(m_dbPath);
        if (!file.good()) {
            std::cerr << "Failed to open database file for reading: " << m_dbPath << std::endl;
            return projects;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) {
                continue;
            }
            
            Project project;
            size_t pos = 0;
            size_t nextPos = 0;
            
            // معرف المشروع


            nextPos = line.find('|', pos);
            if (nextPos == std::string::npos) continue;
            project.id = std::stoi(line.substr(pos, nextPos - pos));
            pos = nextPos + 1;
            
            // اسم المشروع
            nextPos = line.find('|', pos);
            if (nextPos == std::string::npos) continue;
            project.name = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            // مسار المشروع
            nextPos = line.find('|', pos);
            if (nextPos == std::string::npos) continue;
            project.path = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            // مسار الصورة المصغرة
            nextPos = line.find('|', pos);
            if (nextPos == std::string::npos) continue;
            project.thumbnail_path = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            // تاريخ الإنشاء
            nextPos = line.find('|', pos);
            if (nextPos == std::string::npos) continue;
            project.created_at = line.substr(pos, nextPos - pos);
            pos = nextPos + 1;
            
            // تاريخ التحديث
            project.updated_at = line.substr(pos);
            
            projects.push_back(project);
        }
        

        file.close();
        
        return projects;
    }
    
    bool updateProject(int id, const std::string& name, const std::string& path, const std::string& thumbnail_path) {
        if (!m_initialized) {
            return false;
        }
        
        // قراءة المشاريع
        std::vector<Project> projects = getAllProjects();
        
        // البحث عن المشروع وتحديثه
        bool found = false;
        for (auto& project : projects) {
            if (project.id == id) {
                project.name = name;
                project.path = path;
                project.thumbnail_path = thumbnail_path;
                
                // تحديث وقت التحديث
                std::time_t now = std::time(nullptr);
                char timeStr[20];
                std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
                project.updated_at = timeStr;
                
                found = true;
                break;
            }
        }
        
        if (!found) {
            return false;
        }
        
        // إعادة كتابة الملف
        std::ofstream file(m_dbPath, std::ios::trunc);

