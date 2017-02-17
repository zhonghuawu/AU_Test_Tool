#ifndef _IAU_HPP_
#define _IAU_HPP_

 #include "iau.h"
 #include "IAU_Util.h"
 
#if defined _WIN32 || defined _WIN64
 #define WIN32_LEAN_AND_MEAN
 #include <windows.h>
 #undef WIN32_LEAN_AND_MEAN
 #define WIN32_LEAN_AND_MEAN
#else
 #include <cstdlib>
 #include <dlfcn.h>
#endif
 #define MAXPATH 256
 #include <cassert>
 #include <string>

 namespace Iau {
     
     /*
       class Status
       {
       public:
       Status();
         
       Status(IAU_Status status);
 
       IAU_StatusCategory GetCategory() const;
         
       IAU_StatusSeverity GetSeverity() const;
 
       IAU_StatusSeverity GetDetail() const;
         
       IAU_Status GetStatusCode() const;
 
       // Safe Bool Idiom
       operator SafeBool() const;
 
       std::wstring GetMessage() const;
 
       private:
       IAU_Status status_;
 
       // for Safe Bool Idiom
       typedef void (Status::*SafeBool)() const;
       void SafeBoolTrue() const {}
       };
     */
     
     namespace Private {
         class Noncopyable
         {
         protected:
             Noncopyable() {}
             ~Noncopyable() {}
         private:
             Noncopyable( const Noncopyable& );
            const Noncopyable& operator=( const Noncopyable& );
        };
         
	 }

     class Library : Private::Noncopyable
     {
     public:
         explicit Library(const std::wstring & path)
             : path_(path)
             , handle_(NULL)
             , getWideString_(NULL)
             , getWideStringLength_(NULL)
             , createDataHandle_(NULL)
             , destroyDataHandle_(NULL)
             , setWideString_(NULL)
             , startup_(NULL)
             , shutdown_(NULL)
             , getVersion_(NULL)
             , createContext_(NULL)
             , destroyContext_(NULL)
             , subscribeEvent_(NULL)
             , unsubscribeEvent_(NULL)
             , update_(NULL)
             , queryMetaInformation_(NULL)
             , queryUpdateInformation_(NULL)
             , feedback_(NULL)
             , getStatusMessage_(NULL)
         {
         }
 
         ~Library()
         {
             Unload();
         }
 
         bool Load()
         {
             if ( IsValid() ) {
                 return true;
             }
 
#if defined _WIN32 || defined _WIN64
             handle_ = ::LoadLibraryW(path_.c_str());
#else
             handle_ = dlopen(WChar2Ansi(path_.c_str()).c_str(), RTLD_NOW);
#endif
             if ( NULL == handle_ ) {
                 return false;
             }
 
             bool success = 
                 GetFunction_(getWideString_, "IAU_GetWideString") &&
                 GetFunction_(getWideStringLength_, "IAU_GetWideStringLength") &&
                 GetFunction_(createDataHandle_, "IAU_CreateDataHandle") &&
                 GetFunction_(destroyDataHandle_, "IAU_DestroyDataHandle") &&
                 GetFunction_(setWideString_, "IAU_SetWideString") &&
                 GetFunction_(startup_, "IAU_Startup") &&
                 GetFunction_(shutdown_, "IAU_Shutdown") &&
                 GetFunction_(getVersion_, "IAU_GetVersion") &&
                 GetFunction_(createContext_, "IAU_CreateContext") &&
                 GetFunction_(destroyContext_, "IAU_DestroyContext") &&
                 GetFunction_(subscribeEvent_, "IAU_SubscribeEvent") &&
                 GetFunction_(unsubscribeEvent_, "IAU_UnsubscribeEvent") &&
                 GetFunction_(update_, "IAU_Update") &&
 
                 GetFunction_(queryMetaInformation_, "IAU_QueryMetaInformation") &&
                 GetFunction_(queryUpdateInformation_, "IAU_QueryUpdateInformation") &&
                 GetFunction_(feedback_, "IAU_Feedback") &&
                 GetFunction_(getStatusMessage_, "IAU_GetStatusMessage")
                 ;
             
             if ( ! success ) {
                 Unload();
             }
 
             return success;
         }
 
         bool IsValid() const
         {
             return NULL != handle_;
         }
 
         bool Unload()
         {
             if ( ! IsValid() ) {
                 return false;
             }
 
             getWideString_ = NULL;
             getWideStringLength_ = NULL;
             createDataHandle_ = NULL;
             destroyDataHandle_ = NULL;
             setWideString_ = NULL;
             startup_ = NULL;
             shutdown_ = NULL;
             getVersion_ = NULL;
             createContext_ = NULL;
             destroyContext_ = NULL;
             subscribeEvent_ = NULL;
             unsubscribeEvent_ = NULL;
             update_ = NULL;
             queryMetaInformation_ = NULL;
             queryUpdateInformation_ = NULL;
             feedback_ = NULL;
             getStatusMessage_ = NULL;
             
#if defined _WIN32 || defined _WIN64
             ::FreeLibrary(handle_);
#else
             dlclose(handle_);
#endif
 
             handle_ = NULL;
             return true;
         }
         
         std::wstring const& GetPath() const
         {
             return path_;
         }
 
         IAU_Status SetWideString(
             IAU_DataHandle destination,
             std::wstring const& source)
         {
             if ( IsValid() ) {
                 return setWideString_(destination, source.c_str());
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
 
 
         IAU_Status GetWideString(
             IAU_DataHandle source,
             std::wstring& data)
         {
             if ( IsValid() ) {
                 size_t length = 0;
                 IAU_Status status = getWideStringLength_(source, &length);
                 if ( IAU_FAILED( status ) ) {
                     return status;
                 }
                 data.resize(length);
                 return getWideString_(source, &*data.begin(), data.size());
             }
             
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
 
         IAU_Status CreateDataHandle(IAU_DataHandle& newDataHandle)
         {
             if ( IsValid() ) {
                 return createDataHandle_(&newDataHandle);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
         
 
         IAU_Status DestroyDataHandle(IAU_DataHandle dataHandle)
         {
             if ( IsValid() ) {
                 return destroyDataHandle_(dataHandle);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
         
 
         IAU_Status Startup(void* reserved=NULL)
         {
             if ( IsValid() ) {
                 return startup_(reserved);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
         
         IAU_Status Shutdown()
         {
             if ( IsValid() ) {
                 return shutdown_();
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
 
         IAU_Status GetVersion(IAU_Version& version)
         {
             if ( IsValid() ) {
                 return getVersion_(&version);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
 
 
         IAU_Status CreateContext(
             IAU_ContextHandle& context,
             std::wstring const& contextConfig)
         {
             if ( IsValid() ) {
                 return createContext_(&context, contextConfig.c_str());
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
         
 
         IAU_Status DestroyContext(IAU_ContextHandle context)
         {
             if ( IsValid() ) {
                 return destroyContext_(context);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
 
 
         IAU_Status SubscribeEvent(
             IAU_ContextHandle context,
             IAU_EventType event,
             IAU_EventHandler eventHandler,
             void* userData)
         {
             if ( IsValid() ) {
                 return subscribeEvent_(
                     context,
                     event,
                     eventHandler,
                     userData);
             }
 
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
         
 
         IAU_Status UnsubscribeEvent(
             IAU_ContextHandle context,
             IAU_EventType event,
             IAU_EventHandler& oldEventHandler,
             void*& oldUserData)
         {
             if ( IsValid() ) {
                 return unsubscribeEvent_(
                     context,
                     event,
                     &oldEventHandler,
                     &oldUserData);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
 
 
         IAU_Status Update(
             IAU_ContextHandle context,
             std::wstring const& productsInformation,
             std::wstring& updateResult)
         {
			 wchar_t* test1 = (wchar_t*)productsInformation.c_str();
             if ( IsValid() ) {
                 
                 IAU_DataHandle data = NULL;
                 IAU_Status status = CreateDataHandle(data);
 
                 if ( IAU_FAILED( status ) ) {
                     return status;
                 }
 
                 status = update_(
                     context,
                     productsInformation.c_str(),
					 data);
 
                 if ( IAU_FAILED( status ) ) {
                     return status;
                 }
 
                 return GetWideString(data, updateResult);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
         
         IAU_Status QueryMetaInformation(
             IAU_ContextHandle context,
             std::wstring const& productId,
             std::wstring& metaInformation)
         {
             if ( IsValid() ) {
 
                 IAU_DataHandle data = NULL;
                 IAU_Status status = CreateDataHandle(data);
 
                 if ( IAU_FAILED( status ) ) {
                     return status;
                 }
 
                 status = queryMetaInformation_(
                     context,
                     productId.c_str(),
                     data);
 
                 if ( IAU_FAILED( status ) ) {
                     return status;
                 }
 
                 return GetWideString(data, metaInformation);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
         
 
         IAU_Status QueryUpdateInformation(
             IAU_ContextHandle context,
             std::wstring const& productInformation,
             std::wstring& updateInformation)
         {
             if ( IsValid() ) {
                 IAU_DataHandle data = NULL;
                 IAU_Status status = CreateDataHandle(data);
 
                 if ( IAU_FAILED( status ) ) {
                     return status;
                 }
 
                 
                 status = queryUpdateInformation_(
                     context, productInformation.c_str(),
                     data);
 
                 if ( IAU_FAILED( status ) ) {
                     return status;
                 }
                 
                 return GetWideString(data, updateInformation);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
 
         IAU_Status Feedback(
             IAU_ContextHandle context,
             void const* feedbackData,
             size_t dataSize,
             IAU_FeedbackOption option)
         {
             if ( IsValid() ) {
                 return feedback_(context, feedbackData, dataSize, option);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
         
         
         std::wstring GetStatusMessage(IAU_Status status)
         {
             if ( IsValid() ) {
                 IAU_StatusMessage message = getStatusMessage_(status);
                 if ( NULL != message.value ) {
                     return message.value;
                 }
             }
             return std::wstring();
         }
         
 
     private:
 
         template<typename Signature>
         bool GetFunction_(Signature*& function, char const* functionName)
         {
             if ( NULL == handle_ ) {
                 return false;
             }
                 
#if defined _WIN32 || defined _WIN64
             function = reinterpret_cast<Signature*>(::GetProcAddress(handle_, functionName));
#else
             function = (Signature*)(dlsym(handle_, functionName));
#endif
             return NULL != function;
         }
 
         std::wstring const path_;
 
#if defined _WIN32 || defined _WIN64
         ::HMODULE handle_;
#else 
         void* handle_;
#endif
 
         IAU_GetWideString* getWideString_;
         IAU_GetWideStringLength* getWideStringLength_;
         IAU_CreateDataHandle* createDataHandle_;
         IAU_DestroyDataHandle* destroyDataHandle_;
         IAU_SetWideString* setWideString_;
         IAU_Startup* startup_;
         IAU_Shutdown* shutdown_;
         IAU_GetVersion* getVersion_;
         IAU_CreateContext* createContext_;
         IAU_DestroyContext* destroyContext_;
         IAU_SubscribeEvent* subscribeEvent_;
         IAU_UnsubscribeEvent* unsubscribeEvent_;
         IAU_Update* update_;
         IAU_QueryMetaInformation* queryMetaInformation_;
         IAU_QueryUpdateInformation* queryUpdateInformation_;
         IAU_Feedback* feedback_;
         IAU_GetStatusMessage* getStatusMessage_;
     };
 
 
     class Client : Private::Noncopyable
     {
     public:
         Client(Library& library)
             : library_(library)
             , context_(NULL)
         {
         }
 
         bool IsValid() const
         {
             return NULL != context_;
         }
         
         IAU_Status Initialize(std::wstring const& contextConfig)
         {
             IAU_Status status = GetLibrary().CreateContext(
                 context_,
                 contextConfig);
 
             if ( IAU_SUCCEEDED( status ) ) {
                 for ( size_t event = 0;
                       event < IAU_TOTAL_EVENT_TYPE_COUNT;
                       ++ event ) {
                     status = GetLibrary().SubscribeEvent(
                         context_,
                         static_cast<IAU_EventType>(event),
                         &Client::EventHandler,
                         this);
 
                     if ( IAU_FAILED( status ) ) {
                         return status;
                     }
                     
                 }
             }
             return status;
         }
 
         virtual ~Client()
         {
             if ( this->IsValid() ) {
                 this->Deinitialize();
             }
         }
 
         IAU_Status Deinitialize()
         {
             IAU_Status status = GetLibrary().DestroyContext(context_);
 
             if ( IAU_SUCCEEDED( status ) ) {
                 context_ = NULL;
             }
             return status;
         }
 
         Library& GetLibrary()
         {
             return library_;
         }
         
 
         IAU_Status SubscribeEvent(
             IAU_EventType event,
             IAU_EventHandler eventHandler,
             void* userData)
         {
             if ( IsValid() ) {
                 return GetLibrary().SubscribeEvent(
                     context_,
                     event,
                     eventHandler,
                     userData);
             }

             return IAU_STATUS_NOT_IMPLEMENTED;
          }
         
          IAU_Status UnsubscribeEvent(
             IAU_EventType event,             IAU_EventHandler& oldEventHandler,
             void*& oldUserData)
         {
             if ( IsValid() ) {
                 return GetLibrary().UnsubscribeEvent(
                     context_,
                     event,
                     oldEventHandler,
                     oldUserData);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
 
 
         IAU_Status Update(
             std::wstring const& productsInformation,
             std::wstring& updateResult)
         {
             if ( IsValid() ) {
                 return GetLibrary().Update(
                     context_,
                     productsInformation,
                     updateResult);
             }
            return IAU_STATUS_NOT_IMPLEMENTED;
         }
         
 
         IAU_Status QueryMetaInformation(
             std::wstring const& productId,
             std::wstring& metaInformation)
         {
             if ( IsValid() ) {
                 return GetLibrary().QueryMetaInformation(
                     context_,
                     productId,
                     metaInformation);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
         
 
         IAU_Status QueryUpdateInformation(
             std::wstring const& productInformation,
             std::wstring& updateInformation)
         {
             if ( IsValid() ) {
                 return GetLibrary().QueryUpdateInformation(
                     context_,
                     productInformation,
                     updateInformation);
             }
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
 
         IAU_Status Feedback(void const* feedbackData, size_t dataSize, IAU_FeedbackOption option)
         {
             if ( IsValid() ) {
                 return GetLibrary().Feedback(
                     context_,
                    feedbackData,
                     dataSize,
                     option);
             }
             
             return IAU_STATUS_NOT_IMPLEMENTED;
         }
 
         virtual IAU_EventResult OnEvent(IAU_EventType eventType,
                                    IAU_EventMessage& eventMessage)
         {
             return IAU_CONTINUE;
         }
 
     private:
         IAU_ContextHandle context_;
 
         Library& library_;
 
         static IAU_EventResult EventHandler(
             IAU_EventType eventType,
             IAU_EventMessage* eventMessage,
             void* userData)
         {
             Client* client = static_cast<Client*>(userData);
             return client->OnEvent(eventType, *eventMessage);
         }
	 };
 }

 #endif  //_IAU_HPP_
