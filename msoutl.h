// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// CMSOUTL wrapper class

class CMSOUTL : public COleDispatchDriver
{
public:
    CMSOUTL() {}		// Calls COleDispatchDriver default constructor
    CMSOUTL(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CMSOUTL(const CMSOUTL& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetDefaultItemType();
    CString GetDefaultMessageClass();
    CString GetDescription();
    void SetDescription(LPCTSTR lpszNewValue);
    CString GetEntryID();
    LPDISPATCH GetFolders();
    LPDISPATCH GetItems();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    CString GetStoreID();
    long GetUnReadItemCount();
    LPDISPATCH CopyTo(LPDISPATCH DestinationFolder);
    void Delete();
    void Display();
    LPDISPATCH GetExplorer(const VARIANT& DisplayMode);
    void MoveTo(LPDISPATCH DestinationFolder);
    BOOL GetWebViewOn();
    void SetWebViewOn(BOOL bNewValue);
    CString GetWebViewURL();
    void SetWebViewURL(LPCTSTR lpszNewValue);
    void AddToPFFavorites();
    CString GetAddressBookName();
    void SetAddressBookName(LPCTSTR lpszNewValue);
    BOOL GetShowAsOutlookAB();
    void SetShowAsOutlookAB(BOOL bNewValue);
    CString GetFolderPath();
    void AddToFavorites(const VARIANT& fNoUI, const VARIANT& Name);
    BOOL GetInAppFolderSyncObject();
    void SetInAppFolderSyncObject(BOOL bNewValue);
    LPDISPATCH GetCurrentView();
    BOOL GetCustomViewsOnly();
    void SetCustomViewsOnly(BOOL bNewValue);
    LPDISPATCH GetViews();
    BOOL GetIsSharePointFolder();
    long GetShowItemCount();
    void SetShowItemCount(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Action wrapper class

class Action : public COleDispatchDriver
{
public:
    Action() {}		// Calls COleDispatchDriver default constructor
    Action(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Action(const Action& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCopyLike();
    void SetCopyLike(long nNewValue);
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    CString GetPrefix();
    void SetPrefix(LPCTSTR lpszNewValue);
    long GetReplyStyle();
    void SetReplyStyle(long nNewValue);
    long GetResponseStyle();
    void SetResponseStyle(long nNewValue);
    long GetShowOn();
    void SetShowOn(long nNewValue);
    void Delete();
    LPDISPATCH Execute();
};
/////////////////////////////////////////////////////////////////////////////
// _Application wrapper class

class _Application : public COleDispatchDriver
{
public:
    _Application() {}		// Calls COleDispatchDriver default constructor
    _Application(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Application(const _Application& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetAssistant();
    CString GetName();
    CString GetVersion();
    LPDISPATCH ActiveExplorer();
    LPDISPATCH ActiveInspector();
    LPDISPATCH CreateItem(long ItemType);
    LPDISPATCH CreateItemFromTemplate(LPCTSTR TemplatePath, const VARIANT& InFolder);
    LPDISPATCH CreateObject(LPCTSTR ObjectName);
    LPDISPATCH GetNamespace(LPCTSTR Type);
    void Quit();
    LPDISPATCH GetCOMAddIns();
    LPDISPATCH GetExplorers();
    LPDISPATCH GetInspectors();
    LPDISPATCH GetLanguageSettings();
    CString GetProductCode();
    LPDISPATCH GetAnswerWizard();
    LPDISPATCH ActiveWindow();
    LPDISPATCH CopyFile(LPCTSTR FilePath, LPCTSTR DestFolderPath);
    LPDISPATCH AdvancedSearch(LPCTSTR Scope, const VARIANT& Filter, const VARIANT& SearchSubFolders, const VARIANT& Tag);
    BOOL IsSearchSynchronous(LPCTSTR LookInFolders);
    LPDISPATCH GetReminders();
};
/////////////////////////////////////////////////////////////////////////////
// _NameSpace wrapper class

class _NameSpace : public COleDispatchDriver
{
public:
    _NameSpace() {}		// Calls COleDispatchDriver default constructor
    _NameSpace(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _NameSpace(const _NameSpace& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetCurrentUser();
    LPDISPATCH GetFolders();
    CString GetType();
    LPDISPATCH GetAddressLists();
    LPDISPATCH CreateRecipient(LPCTSTR RecipientName);
    LPDISPATCH GetDefaultFolder(long FolderType);
    LPDISPATCH GetFolderFromID(LPCTSTR EntryIDFolder, const VARIANT& EntryIDStore);
    LPDISPATCH GetItemFromID(LPCTSTR EntryIDItem, const VARIANT& EntryIDStore);
    LPDISPATCH GetRecipientFromID(LPCTSTR EntryID);
    LPDISPATCH GetSharedDefaultFolder(LPDISPATCH Recipient, long FolderType);
    void Logoff();
    void Logon(const VARIANT& Profile, const VARIANT& Password, const VARIANT& ShowDialog, const VARIANT& NewSession);
    LPDISPATCH PickFolder();
    LPDISPATCH GetSyncObjects();
    void AddStore(const VARIANT& Store);
    void RemoveStore(LPDISPATCH Folder);
    BOOL GetOffline();
    void Dial(const VARIANT& ContactItem);
    long GetExchangeConnectionMode();
    void AddStoreEx(const VARIANT& Store, long Type);
};
/////////////////////////////////////////////////////////////////////////////
// Recipient wrapper class

class Recipient : public COleDispatchDriver
{
public:
    Recipient() {}		// Calls COleDispatchDriver default constructor
    Recipient(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Recipient(const Recipient& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    CString GetAddress();
    LPDISPATCH GetAddressEntry();
    void SetRefAddressEntry(LPDISPATCH newValue);
    CString GetAutoResponse();
    void SetAutoResponse(LPCTSTR lpszNewValue);
    long GetDisplayType();
    CString GetEntryID();
    long GetIndex();
    long GetMeetingResponseStatus();
    CString GetName();
    BOOL GetResolved();
    long GetTrackingStatus();
    void SetTrackingStatus(long nNewValue);
    DATE GetTrackingStatusTime();
    void SetTrackingStatusTime(DATE newValue);
    long GetType();
    void SetType(long nNewValue);
    void Delete();
    CString FreeBusy(DATE Start, long MinPerChar, const VARIANT& CompleteFormat);
    BOOL Resolve();
};
/////////////////////////////////////////////////////////////////////////////
// AddressEntry wrapper class

class AddressEntry : public COleDispatchDriver
{
public:
    AddressEntry() {}		// Calls COleDispatchDriver default constructor
    AddressEntry(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AddressEntry(const AddressEntry& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    CString GetAddress();
    void SetAddress(LPCTSTR lpszNewValue);
    long GetDisplayType();
    CString GetId();
    LPDISPATCH GetManager();
    LPDISPATCH GetMembers();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    CString GetType();
    void SetType(LPCTSTR lpszNewValue);
    void Delete();
    void Details(const VARIANT& HWnd);
    CString GetFreeBusy(DATE Start, long MinPerChar, const VARIANT& CompleteFormat);
    void Update(const VARIANT& MakePermanent, const VARIANT& Refresh);
};
/////////////////////////////////////////////////////////////////////////////
// AddressEntries wrapper class

class AddressEntries : public COleDispatchDriver
{
public:
    AddressEntries() {}		// Calls COleDispatchDriver default constructor
    AddressEntries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AddressEntries(const AddressEntries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(LPCTSTR Type, const VARIANT& Name, const VARIANT& Address);
    LPDISPATCH GetFirst();
    LPDISPATCH GetLast();
    LPDISPATCH GetNext();
    LPDISPATCH GetPrevious();
    void Sort(const VARIANT& Property_, const VARIANT& Order);
};
/////////////////////////////////////////////////////////////////////////////
// _Folders wrapper class

class _Folders : public COleDispatchDriver
{
public:
    _Folders() {}		// Calls COleDispatchDriver default constructor
    _Folders(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Folders(const _Folders& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(LPCTSTR Name, const VARIANT& Type);
    LPDISPATCH GetFirst();
    LPDISPATCH GetLast();
    LPDISPATCH GetNext();
    LPDISPATCH GetPrevious();
    void Remove(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// MAPIFolder wrapper class

class MAPIFolder : public COleDispatchDriver
{
public:
    MAPIFolder() {}		// Calls COleDispatchDriver default constructor
    MAPIFolder(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    MAPIFolder(const MAPIFolder& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetDefaultItemType();
    CString GetDefaultMessageClass();
    CString GetDescription();
    void SetDescription(LPCTSTR lpszNewValue);
    CString GetEntryID();
    LPDISPATCH GetFolders();
    LPDISPATCH GetItems();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    CString GetStoreID();
    long GetUnReadItemCount();
    LPDISPATCH CopyTo(LPDISPATCH DestinationFolder);
    void Delete();
    void Display();
    LPDISPATCH GetExplorer(const VARIANT& DisplayMode);
    void MoveTo(LPDISPATCH DestinationFolder);
    BOOL GetWebViewOn();
    void SetWebViewOn(BOOL bNewValue);
    CString GetWebViewURL();
    void SetWebViewURL(LPCTSTR lpszNewValue);
    void AddToPFFavorites();
    CString GetAddressBookName();
    void SetAddressBookName(LPCTSTR lpszNewValue);
    BOOL GetShowAsOutlookAB();
    void SetShowAsOutlookAB(BOOL bNewValue);
    CString GetFolderPath();
    void AddToFavorites(const VARIANT& fNoUI, const VARIANT& Name);
    BOOL GetInAppFolderSyncObject();
    void SetInAppFolderSyncObject(BOOL bNewValue);
    LPDISPATCH GetCurrentView();
    BOOL GetCustomViewsOnly();
    void SetCustomViewsOnly(BOOL bNewValue);
    LPDISPATCH GetViews();
    BOOL GetIsSharePointFolder();
    long GetShowItemCount();
    void SetShowItemCount(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// _Items wrapper class

class _Items : public COleDispatchDriver
{
public:
    _Items() {}		// Calls COleDispatchDriver default constructor
    _Items(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Items(const _Items& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    BOOL GetIncludeRecurrences();
    void SetIncludeRecurrences(BOOL bNewValue);
    LPDISPATCH Add(const VARIANT& Type);
    LPDISPATCH Find(LPCTSTR Filter);
    LPDISPATCH FindNext();
    LPDISPATCH GetFirst();
    LPDISPATCH GetLast();
    LPDISPATCH GetNext();
    LPDISPATCH GetPrevious();
    void Remove(long Index);
    void ResetColumns();
    LPDISPATCH Restrict(LPCTSTR Filter);
    void SetColumns(LPCTSTR Columns);
    void Sort(LPCTSTR Property_, const VARIANT& Descending);
};
/////////////////////////////////////////////////////////////////////////////
// _Explorer wrapper class

class _Explorer : public COleDispatchDriver
{
public:
    _Explorer() {}		// Calls COleDispatchDriver default constructor
    _Explorer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Explorer(const _Explorer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetCommandBars();
    LPDISPATCH GetCurrentFolder();
    void SetRefCurrentFolder(LPDISPATCH newValue);
    void Close();
    void Display();
    CString GetCaption();
    VARIANT GetCurrentView();
    void SetCurrentView(const VARIANT& newValue);
    long GetHeight();
    void SetHeight(long nNewValue);
    long GetLeft();
    void SetLeft(long nNewValue);
    LPDISPATCH GetPanes();
    LPDISPATCH GetSelection();
    long GetTop();
    void SetTop(long nNewValue);
    long GetWidth();
    void SetWidth(long nNewValue);
    long GetWindowState();
    void SetWindowState(long nNewValue);
    void Activate();
    BOOL IsPaneVisible(long Pane);
    void ShowPane(long Pane, BOOL Visible);
    LPDISPATCH GetHTMLDocument();
    void SelectFolder(LPDISPATCH Folder);
    void DeselectFolder(LPDISPATCH Folder);
    BOOL IsFolderSelected(LPDISPATCH Folder);
};
/////////////////////////////////////////////////////////////////////////////
// Panes wrapper class

class Panes : public COleDispatchDriver
{
public:
    Panes() {}		// Calls COleDispatchDriver default constructor
    Panes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Panes(const Panes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// Selection wrapper class

class Selection : public COleDispatchDriver
{
public:
    Selection() {}		// Calls COleDispatchDriver default constructor
    Selection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Selection(const Selection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// View wrapper class

class View : public COleDispatchDriver
{
public:
    View() {}		// Calls COleDispatchDriver default constructor
    View(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    View(const View& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    void Apply();
    LPDISPATCH Copy(LPCTSTR Name, long SaveOption);
    void Delete();
    void Reset();
    void Save();
    CString GetLanguage();
    void SetLanguage(LPCTSTR lpszNewValue);
    BOOL GetLockUserChanges();
    void SetLockUserChanges(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    long GetSaveOption();
    BOOL GetStandard();
    long GetViewType();
    CString GetXml();
    void SetXml(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// _Views wrapper class

class _Views : public COleDispatchDriver
{
public:
    _Views() {}		// Calls COleDispatchDriver default constructor
    _Views(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Views(const _Views& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(LPCTSTR Name, long ViewType, long SaveOption);
    void Remove(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// AddressLists wrapper class

class AddressLists : public COleDispatchDriver
{
public:
    AddressLists() {}		// Calls COleDispatchDriver default constructor
    AddressLists(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AddressLists(const AddressLists& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// AddressList wrapper class

class AddressList : public COleDispatchDriver
{
public:
    AddressList() {}		// Calls COleDispatchDriver default constructor
    AddressList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AddressList(const AddressList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetAddressEntries();
    CString GetId();
    long GetIndex();
    BOOL GetIsReadOnly();
    CString GetName();
};
/////////////////////////////////////////////////////////////////////////////
// SyncObjects wrapper class

class SyncObjects : public COleDispatchDriver
{
public:
    SyncObjects() {}		// Calls COleDispatchDriver default constructor
    SyncObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SyncObjects(const SyncObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH GetAppFolders();
};
/////////////////////////////////////////////////////////////////////////////
// _SyncObject wrapper class

class _SyncObject : public COleDispatchDriver
{
public:
    _SyncObject() {}		// Calls COleDispatchDriver default constructor
    _SyncObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _SyncObject(const _SyncObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    CString GetName();
    void Start();
    void Stop();
};
/////////////////////////////////////////////////////////////////////////////
// SyncObjectEvents wrapper class

class SyncObjectEvents : public COleDispatchDriver
{
public:
    SyncObjectEvents() {}		// Calls COleDispatchDriver default constructor
    SyncObjectEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SyncObjectEvents(const SyncObjectEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void SyncStart();
    void Progress(long State, LPCTSTR Description, long Value, long Max);
    void OnError(long Code, LPCTSTR Description);
    void SyncEnd();
};
/////////////////////////////////////////////////////////////////////////////
// _Inspector wrapper class

class _Inspector : public COleDispatchDriver
{
public:
    _Inspector() {}		// Calls COleDispatchDriver default constructor
    _Inspector(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Inspector(const _Inspector& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetCommandBars();
    LPDISPATCH GetCurrentItem();
    long GetEditorType();
    LPDISPATCH GetModifiedFormPages();
    void Close(long SaveMode);
    void Display(const VARIANT& Modal);
    void HideFormPage(LPCTSTR PageName);
    BOOL IsWordMail();
    void SetCurrentFormPage(LPCTSTR PageName);
    void ShowFormPage(LPCTSTR PageName);
    LPDISPATCH GetHTMLEditor();
    LPDISPATCH GetWordEditor();
    CString GetCaption();
    long GetHeight();
    void SetHeight(long nNewValue);
    long GetLeft();
    void SetLeft(long nNewValue);
    long GetTop();
    void SetTop(long nNewValue);
    long GetWidth();
    void SetWidth(long nNewValue);
    long GetWindowState();
    void SetWindowState(long nNewValue);
    void Activate();
    void SetControlItemProperty(LPDISPATCH Control, LPCTSTR PropertyName);
};
/////////////////////////////////////////////////////////////////////////////
// _Explorers wrapper class

class _Explorers : public COleDispatchDriver
{
public:
    _Explorers() {}		// Calls COleDispatchDriver default constructor
    _Explorers(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Explorers(const _Explorers& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(const VARIANT& Folder, long DisplayMode);
};
/////////////////////////////////////////////////////////////////////////////
// ExplorerEvents wrapper class

class ExplorerEvents : public COleDispatchDriver
{
public:
    ExplorerEvents() {}		// Calls COleDispatchDriver default constructor
    ExplorerEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ExplorerEvents(const ExplorerEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void Activate();
    void FolderSwitch();
    void BeforeFolderSwitch(LPDISPATCH NewFolder, BOOL* Cancel);
    void ViewSwitch();
    void BeforeViewSwitch(const VARIANT& NewView, BOOL* Cancel);
    void Deactivate();
    void SelectionChange();
    void Close();
};
/////////////////////////////////////////////////////////////////////////////
// ExplorerEvents_10 wrapper class

class ExplorerEvents_10 : public COleDispatchDriver
{
public:
    ExplorerEvents_10() {}		// Calls COleDispatchDriver default constructor
    ExplorerEvents_10(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ExplorerEvents_10(const ExplorerEvents_10& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    // method 'Activate' not emitted because of invalid return type or parameter type
    // method 'FolderSwitch' not emitted because of invalid return type or parameter type
    // method 'BeforeFolderSwitch' not emitted because of invalid return type or parameter type
    // method 'ViewSwitch' not emitted because of invalid return type or parameter type
    // method 'BeforeViewSwitch' not emitted because of invalid return type or parameter type
    // method 'Deactivate' not emitted because of invalid return type or parameter type
    // method 'SelectionChange' not emitted because of invalid return type or parameter type
    // method 'Close' not emitted because of invalid return type or parameter type
    // method 'BeforeMaximize' not emitted because of invalid return type or parameter type
    // method 'BeforeMinimize' not emitted because of invalid return type or parameter type
    // method 'BeforeMove' not emitted because of invalid return type or parameter type
    // method 'BeforeSize' not emitted because of invalid return type or parameter type
    void BeforeItemCopy(BOOL* Cancel);
    void BeforeItemCut(BOOL* Cancel);
    void BeforeItemPaste(VARIANT* ClipboardContent, LPDISPATCH Target, BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// _Inspectors wrapper class

class _Inspectors : public COleDispatchDriver
{
public:
    _Inspectors() {}		// Calls COleDispatchDriver default constructor
    _Inspectors(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Inspectors(const _Inspectors& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(LPDISPATCH Item);
};
/////////////////////////////////////////////////////////////////////////////
// InspectorEvents wrapper class

class InspectorEvents : public COleDispatchDriver
{
public:
    InspectorEvents() {}		// Calls COleDispatchDriver default constructor
    InspectorEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    InspectorEvents(const InspectorEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void Activate();
    void Deactivate();
    void Close();
};
/////////////////////////////////////////////////////////////////////////////
// InspectorEvents_10 wrapper class

class InspectorEvents_10 : public COleDispatchDriver
{
public:
    InspectorEvents_10() {}		// Calls COleDispatchDriver default constructor
    InspectorEvents_10(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    InspectorEvents_10(const InspectorEvents_10& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    // method 'Activate' not emitted because of invalid return type or parameter type
    // method 'Deactivate' not emitted because of invalid return type or parameter type
    // method 'Close' not emitted because of invalid return type or parameter type
    // method 'BeforeMaximize' not emitted because of invalid return type or parameter type
    // method 'BeforeMinimize' not emitted because of invalid return type or parameter type
    // method 'BeforeMove' not emitted because of invalid return type or parameter type
    // method 'BeforeSize' not emitted because of invalid return type or parameter type
};
/////////////////////////////////////////////////////////////////////////////
// Search wrapper class

class Search : public COleDispatchDriver
{
public:
    Search() {}		// Calls COleDispatchDriver default constructor
    Search(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Search(const Search& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    CString GetFilter();
    BOOL GetIsSynchronous();
    LPDISPATCH GetResults();
    BOOL GetSearchSubFolders();
    void Stop();
    CString GetTag();
    CString GetScope();
    LPDISPATCH Save(LPCTSTR Name);
};
/////////////////////////////////////////////////////////////////////////////
// _Results wrapper class

class _Results : public COleDispatchDriver
{
public:
    _Results() {}		// Calls COleDispatchDriver default constructor
    _Results(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Results(const _Results& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH GetFirst();
    LPDISPATCH GetLast();
    LPDISPATCH GetNext();
    LPDISPATCH GetPrevious();
    void ResetColumns();
    void SetColumns(LPCTSTR Columns);
    void Sort(LPCTSTR Property_, const VARIANT& Descending);
    long GetDefaultItemType();
    void SetDefaultItemType(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// _Reminders wrapper class

class _Reminders : public COleDispatchDriver
{
public:
    _Reminders() {}		// Calls COleDispatchDriver default constructor
    _Reminders(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Reminders(const _Reminders& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    void Remove(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// _Reminder wrapper class

class _Reminder : public COleDispatchDriver
{
public:
    _Reminder() {}		// Calls COleDispatchDriver default constructor
    _Reminder(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Reminder(const _Reminder& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    void Dismiss();
    void Snooze(const VARIANT& SnoozeTime);
    CString GetCaption();
    BOOL GetIsVisible();
    LPDISPATCH GetItem();
    DATE GetNextReminderDate();
    DATE GetOriginalReminderDate();
};
/////////////////////////////////////////////////////////////////////////////
// Actions wrapper class

class Actions : public COleDispatchDriver
{
public:
    Actions() {}		// Calls COleDispatchDriver default constructor
    Actions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Actions(const Actions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add();
    void Remove(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// ApplicationEvents wrapper class

class ApplicationEvents : public COleDispatchDriver
{
public:
    ApplicationEvents() {}		// Calls COleDispatchDriver default constructor
    ApplicationEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ApplicationEvents(const ApplicationEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void ItemSend(LPDISPATCH Item, BOOL* Cancel);
    void NewMail();
    void Reminder(LPDISPATCH Item);
    void OptionsPagesAdd(LPDISPATCH Pages);
    void Startup();
    void Quit();
};
/////////////////////////////////////////////////////////////////////////////
// PropertyPages wrapper class

class PropertyPages : public COleDispatchDriver
{
public:
    PropertyPages() {}		// Calls COleDispatchDriver default constructor
    PropertyPages(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PropertyPages(const PropertyPages& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    void Add(const VARIANT& Page, LPCTSTR Title);
    void Remove(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// _AppointmentItem wrapper class

class _AppointmentItem : public COleDispatchDriver
{
public:
    _AppointmentItem() {}		// Calls COleDispatchDriver default constructor
    _AppointmentItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _AppointmentItem(const _AppointmentItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    BOOL GetAllDayEvent();
    void SetAllDayEvent(BOOL bNewValue);
    long GetBusyStatus();
    void SetBusyStatus(long nNewValue);
    long GetDuration();
    void SetDuration(long nNewValue);
    DATE GetEnd();
    void SetEnd(DATE newValue);
    BOOL GetIsOnlineMeeting();
    void SetIsOnlineMeeting(BOOL bNewValue);
    BOOL GetIsRecurring();
    CString GetLocation();
    void SetLocation(LPCTSTR lpszNewValue);
    long GetMeetingStatus();
    void SetMeetingStatus(long nNewValue);
    BOOL GetNetMeetingAutoStart();
    void SetNetMeetingAutoStart(BOOL bNewValue);
    CString GetNetMeetingOrganizerAlias();
    void SetNetMeetingOrganizerAlias(LPCTSTR lpszNewValue);
    CString GetNetMeetingServer();
    void SetNetMeetingServer(LPCTSTR lpszNewValue);
    long GetNetMeetingType();
    void SetNetMeetingType(long nNewValue);
    CString GetOptionalAttendees();
    void SetOptionalAttendees(LPCTSTR lpszNewValue);
    CString GetOrganizer();
    LPDISPATCH GetRecipients();
    long GetRecurrenceState();
    long GetReminderMinutesBeforeStart();
    void SetReminderMinutesBeforeStart(long nNewValue);
    BOOL GetReminderOverrideDefault();
    void SetReminderOverrideDefault(BOOL bNewValue);
    BOOL GetReminderPlaySound();
    void SetReminderPlaySound(BOOL bNewValue);
    BOOL GetReminderSet();
    void SetReminderSet(BOOL bNewValue);
    CString GetReminderSoundFile();
    void SetReminderSoundFile(LPCTSTR lpszNewValue);
    DATE GetReplyTime();
    void SetReplyTime(DATE newValue);
    CString GetRequiredAttendees();
    void SetRequiredAttendees(LPCTSTR lpszNewValue);
    CString GetResources();
    void SetResources(LPCTSTR lpszNewValue);
    BOOL GetResponseRequested();
    void SetResponseRequested(BOOL bNewValue);
    long GetResponseStatus();
    DATE GetStart();
    void SetStart(DATE newValue);
    void ClearRecurrencePattern();
    LPDISPATCH ForwardAsVcal();
    LPDISPATCH GetRecurrencePattern();
    LPDISPATCH Respond(long Response, const VARIANT& fNoUI, const VARIANT& fAdditionalTextDialog);
    void Send();
    CString GetNetMeetingDocPathName();
    void SetNetMeetingDocPathName(LPCTSTR lpszNewValue);
    CString GetNetShowURL();
    void SetNetShowURL(LPCTSTR lpszNewValue);
    LPDISPATCH GetLinks();
    BOOL GetConferenceServerAllowExternal();
    void SetConferenceServerAllowExternal(BOOL bNewValue);
    CString GetConferenceServerPassword();
    void SetConferenceServerPassword(LPCTSTR lpszNewValue);
    LPDISPATCH GetItemProperties();
    long GetDownloadState();
    void ShowCategoriesDialog();
    long GetInternetCodepage();
    void SetInternetCodepage(long nNewValue);
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    CString GetMeetingWorkspaceURL();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
};
/////////////////////////////////////////////////////////////////////////////
// Attachments wrapper class

class Attachments : public COleDispatchDriver
{
public:
    Attachments() {}		// Calls COleDispatchDriver default constructor
    Attachments(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Attachments(const Attachments& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(const VARIANT& Source, const VARIANT& Type, const VARIANT& Position, const VARIANT& DisplayName);
    void Remove(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// Attachment wrapper class

class Attachment : public COleDispatchDriver
{
public:
    Attachment() {}		// Calls COleDispatchDriver default constructor
    Attachment(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Attachment(const Attachment& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    CString GetDisplayName();
    void SetDisplayName(LPCTSTR lpszNewValue);
    CString GetFileName();
    long GetIndex();
    CString GetPathName();
    long GetPosition();
    void SetPosition(long nNewValue);
    long GetType();
    void Delete();
    void SaveAsFile(LPCTSTR Path);
};
/////////////////////////////////////////////////////////////////////////////
// FormDescription wrapper class

class FormDescription : public COleDispatchDriver
{
public:
    FormDescription() {}		// Calls COleDispatchDriver default constructor
    FormDescription(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    FormDescription(const FormDescription& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    CString GetCategory();
    void SetCategory(LPCTSTR lpszNewValue);
    CString GetCategorySub();
    void SetCategorySub(LPCTSTR lpszNewValue);
    CString GetComment();
    void SetComment(LPCTSTR lpszNewValue);
    CString GetContactName();
    void SetContactName(LPCTSTR lpszNewValue);
    CString GetDisplayName();
    void SetDisplayName(LPCTSTR lpszNewValue);
    BOOL GetHidden();
    void SetHidden(BOOL bNewValue);
    CString GetIcon();
    void SetIcon(LPCTSTR lpszNewValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetMessageClass();
    CString GetMiniIcon();
    void SetMiniIcon(LPCTSTR lpszNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    CString GetNumber();
    void SetNumber(LPCTSTR lpszNewValue);
    BOOL GetOneOff();
    void SetOneOff(BOOL bNewValue);
    CString GetPassword();
    void SetPassword(LPCTSTR lpszNewValue);
    CString GetScriptText();
    CString GetTemplate();
    void SetTemplate(LPCTSTR lpszNewValue);
    BOOL GetUseWordMail();
    void SetUseWordMail(BOOL bNewValue);
    CString GetVersion();
    void SetVersion(LPCTSTR lpszNewValue);
    void PublishForm(long Registry, const VARIANT& Folder);
};
/////////////////////////////////////////////////////////////////////////////
// UserProperties wrapper class

class UserProperties : public COleDispatchDriver
{
public:
    UserProperties() {}		// Calls COleDispatchDriver default constructor
    UserProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    UserProperties(const UserProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(LPCTSTR Name, long Type, const VARIANT& AddToFolderFields, const VARIANT& DisplayFormat);
    LPDISPATCH Find(LPCTSTR Name, const VARIANT& Custom);
    void Remove(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// UserProperty wrapper class

class UserProperty : public COleDispatchDriver
{
public:
    UserProperty() {}		// Calls COleDispatchDriver default constructor
    UserProperty(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    UserProperty(const UserProperty& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    CString GetName();
    long GetType();
    CString GetValidationFormula();
    void SetValidationFormula(LPCTSTR lpszNewValue);
    CString GetValidationText();
    void SetValidationText(LPCTSTR lpszNewValue);
    VARIANT GetValue();
    void SetValue(const VARIANT& newValue);
    void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// Recipients wrapper class

class Recipients : public COleDispatchDriver
{
public:
    Recipients() {}		// Calls COleDispatchDriver default constructor
    Recipients(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Recipients(const Recipients& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(LPCTSTR Name);
    void Remove(long Index);
    BOOL ResolveAll();
};
/////////////////////////////////////////////////////////////////////////////
// _MailItem wrapper class

class _MailItem : public COleDispatchDriver
{
public:
    _MailItem() {}		// Calls COleDispatchDriver default constructor
    _MailItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _MailItem(const _MailItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    BOOL GetAlternateRecipientAllowed();
    void SetAlternateRecipientAllowed(BOOL bNewValue);
    BOOL GetAutoForwarded();
    void SetAutoForwarded(BOOL bNewValue);
    CString GetBcc();
    void SetBcc(LPCTSTR lpszNewValue);
    CString GetCc();
    void SetCc(LPCTSTR lpszNewValue);
    DATE GetDeferredDeliveryTime();
    void SetDeferredDeliveryTime(DATE newValue);
    BOOL GetDeleteAfterSubmit();
    void SetDeleteAfterSubmit(BOOL bNewValue);
    DATE GetExpiryTime();
    void SetExpiryTime(DATE newValue);
    DATE GetFlagDueBy();
    void SetFlagDueBy(DATE newValue);
    CString GetFlagRequest();
    void SetFlagRequest(LPCTSTR lpszNewValue);
    long GetFlagStatus();
    void SetFlagStatus(long nNewValue);
    CString GetHTMLBody();
    void SetHTMLBody(LPCTSTR lpszNewValue);
    BOOL GetOriginatorDeliveryReportRequested();
    void SetOriginatorDeliveryReportRequested(BOOL bNewValue);
    BOOL GetReadReceiptRequested();
    void SetReadReceiptRequested(BOOL bNewValue);
    CString GetReceivedByEntryID();
    CString GetReceivedByName();
    CString GetReceivedOnBehalfOfEntryID();
    CString GetReceivedOnBehalfOfName();
    DATE GetReceivedTime();
    BOOL GetRecipientReassignmentProhibited();
    void SetRecipientReassignmentProhibited(BOOL bNewValue);
    LPDISPATCH GetRecipients();
    BOOL GetReminderOverrideDefault();
    void SetReminderOverrideDefault(BOOL bNewValue);
    BOOL GetReminderPlaySound();
    void SetReminderPlaySound(BOOL bNewValue);
    BOOL GetReminderSet();
    void SetReminderSet(BOOL bNewValue);
    CString GetReminderSoundFile();
    void SetReminderSoundFile(LPCTSTR lpszNewValue);
    DATE GetReminderTime();
    void SetReminderTime(DATE newValue);
    long GetRemoteStatus();
    void SetRemoteStatus(long nNewValue);
    CString GetReplyRecipientNames();
    LPDISPATCH GetReplyRecipients();
    LPDISPATCH GetSaveSentMessageFolder();
    void SetRefSaveSentMessageFolder(LPDISPATCH newValue);
    CString GetSenderName();
    BOOL GetSent();
    DATE GetSentOn();
    CString GetSentOnBehalfOfName();
    void SetSentOnBehalfOfName(LPCTSTR lpszNewValue);
    BOOL GetSubmitted();
    CString GetTo();
    void SetTo(LPCTSTR lpszNewValue);
    CString GetVotingOptions();
    void SetVotingOptions(LPCTSTR lpszNewValue);
    CString GetVotingResponse();
    void SetVotingResponse(LPCTSTR lpszNewValue);
    void ClearConversationIndex();
    LPDISPATCH Forward();
    LPDISPATCH Reply();
    LPDISPATCH ReplyAll();
    void Send();
    LPDISPATCH GetLinks();
    LPDISPATCH GetItemProperties();
    long GetBodyFormat();
    void SetBodyFormat(long nNewValue);
    long GetDownloadState();
    void ShowCategoriesDialog();
    long GetInternetCodepage();
    void SetInternetCodepage(long nNewValue);
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    long GetFlagIcon();
    void SetFlagIcon(long nNewValue);
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
    CString GetSenderEmailAddress();
    CString GetSenderEmailType();
    long GetPermission();
    void SetPermission(long nNewValue);
    long GetPermissionService();
    void SetPermissionService(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Links wrapper class

class Links : public COleDispatchDriver
{
public:
    Links() {}		// Calls COleDispatchDriver default constructor
    Links(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Links(const Links& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(LPDISPATCH Item);
    void Remove(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// Link wrapper class

class Link : public COleDispatchDriver
{
public:
    Link() {}		// Calls COleDispatchDriver default constructor
    Link(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Link(const Link& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    CString GetName();
    long GetType();
    LPDISPATCH GetItem();
};
/////////////////////////////////////////////////////////////////////////////
// ItemProperties wrapper class

class ItemProperties : public COleDispatchDriver
{
public:
    ItemProperties() {}		// Calls COleDispatchDriver default constructor
    ItemProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ItemProperties(const ItemProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(LPCTSTR Name, long Type, const VARIANT& AddToFolderFields, const VARIANT& DisplayFormat);
    void Remove(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// ItemProperty wrapper class

class ItemProperty : public COleDispatchDriver
{
public:
    ItemProperty() {}		// Calls COleDispatchDriver default constructor
    ItemProperty(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ItemProperty(const ItemProperty& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    CString GetName();
    long GetType();
    VARIANT GetValue();
    void SetValue(const VARIANT& newValue);
    BOOL GetIsUserProperty();
};
/////////////////////////////////////////////////////////////////////////////
// Conflicts wrapper class

class Conflicts : public COleDispatchDriver
{
public:
    Conflicts() {}		// Calls COleDispatchDriver default constructor
    Conflicts(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Conflicts(const Conflicts& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH GetFirst();
    LPDISPATCH GetLast();
    LPDISPATCH GetNext();
    LPDISPATCH GetPrevious();
};
/////////////////////////////////////////////////////////////////////////////
// Conflict wrapper class

class Conflict : public COleDispatchDriver
{
public:
    Conflict() {}		// Calls COleDispatchDriver default constructor
    Conflict(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Conflict(const Conflict& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetItem();
    CString GetName();
    long GetType();
};
/////////////////////////////////////////////////////////////////////////////
// ItemEvents wrapper class

class ItemEvents : public COleDispatchDriver
{
public:
    ItemEvents() {}		// Calls COleDispatchDriver default constructor
    ItemEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ItemEvents(const ItemEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void Open(BOOL* Cancel);
    void CustomAction(LPDISPATCH Action, LPDISPATCH Response, BOOL* Cancel);
    void CustomPropertyChange(LPCTSTR Name);
    void Forward(LPDISPATCH Forward, BOOL* Cancel);
    void Close(BOOL* Cancel);
    void PropertyChange(LPCTSTR Name);
    void Read();
    void Reply(LPDISPATCH Response, BOOL* Cancel);
    void ReplyAll(LPDISPATCH Response, BOOL* Cancel);
    void Send(BOOL* Cancel);
    void Write(BOOL* Cancel);
    void BeforeCheckNames(BOOL* Cancel);
    void AttachmentAdd(LPDISPATCH Attachment);
    void AttachmentRead(LPDISPATCH Attachment);
    void BeforeAttachmentSave(LPDISPATCH Attachment, BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// ItemEvents_10 wrapper class

class ItemEvents_10 : public COleDispatchDriver
{
public:
    ItemEvents_10() {}		// Calls COleDispatchDriver default constructor
    ItemEvents_10(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ItemEvents_10(const ItemEvents_10& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    // method 'Open' not emitted because of invalid return type or parameter type
    // method 'CustomAction' not emitted because of invalid return type or parameter type
    // method 'CustomPropertyChange' not emitted because of invalid return type or parameter type
    // method 'Forward' not emitted because of invalid return type or parameter type
    // method 'Close' not emitted because of invalid return type or parameter type
    // method 'PropertyChange' not emitted because of invalid return type or parameter type
    // method 'Read' not emitted because of invalid return type or parameter type
    // method 'Reply' not emitted because of invalid return type or parameter type
    // method 'ReplyAll' not emitted because of invalid return type or parameter type
    // method 'Send' not emitted because of invalid return type or parameter type
    // method 'Write' not emitted because of invalid return type or parameter type
    // method 'BeforeCheckNames' not emitted because of invalid return type or parameter type
    // method 'AttachmentAdd' not emitted because of invalid return type or parameter type
    // method 'AttachmentRead' not emitted because of invalid return type or parameter type
    // method 'BeforeAttachmentSave' not emitted because of invalid return type or parameter type
    void BeforeDelete(LPDISPATCH Item, BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// RecurrencePattern wrapper class

class RecurrencePattern : public COleDispatchDriver
{
public:
    RecurrencePattern() {}		// Calls COleDispatchDriver default constructor
    RecurrencePattern(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    RecurrencePattern(const RecurrencePattern& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetDayOfMonth();
    void SetDayOfMonth(long nNewValue);
    long GetDayOfWeekMask();
    void SetDayOfWeekMask(long nNewValue);
    long GetDuration();
    void SetDuration(long nNewValue);
    DATE GetEndTime();
    void SetEndTime(DATE newValue);
    LPDISPATCH GetExceptions();
    long GetInstance();
    void SetInstance(long nNewValue);
    long GetInterval();
    void SetInterval(long nNewValue);
    long GetMonthOfYear();
    void SetMonthOfYear(long nNewValue);
    BOOL GetNoEndDate();
    void SetNoEndDate(BOOL bNewValue);
    long GetOccurrences();
    void SetOccurrences(long nNewValue);
    DATE GetPatternEndDate();
    void SetPatternEndDate(DATE newValue);
    DATE GetPatternStartDate();
    void SetPatternStartDate(DATE newValue);
    long GetRecurrenceType();
    void SetRecurrenceType(long nNewValue);
    BOOL GetRegenerate();
    void SetRegenerate(BOOL bNewValue);
    DATE GetStartTime();
    void SetStartTime(DATE newValue);
    LPDISPATCH GetOccurrence(DATE StartDate);
};
/////////////////////////////////////////////////////////////////////////////
// Exceptions wrapper class

class Exceptions : public COleDispatchDriver
{
public:
    Exceptions() {}		// Calls COleDispatchDriver default constructor
    Exceptions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Exceptions(const Exceptions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// Exception wrapper class

class Exception : public COleDispatchDriver
{
public:
    Exception() {}		// Calls COleDispatchDriver default constructor
    Exception(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Exception(const Exception& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetAppointmentItem();
    BOOL GetDeleted();
    DATE GetOriginalDate();
    LPDISPATCH GetItemProperties();
};
/////////////////////////////////////////////////////////////////////////////
// _MeetingItem wrapper class

class _MeetingItem : public COleDispatchDriver
{
public:
    _MeetingItem() {}		// Calls COleDispatchDriver default constructor
    _MeetingItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _MeetingItem(const _MeetingItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    BOOL GetAutoForwarded();
    void SetAutoForwarded(BOOL bNewValue);
    DATE GetDeferredDeliveryTime();
    void SetDeferredDeliveryTime(DATE newValue);
    BOOL GetDeleteAfterSubmit();
    void SetDeleteAfterSubmit(BOOL bNewValue);
    DATE GetExpiryTime();
    void SetExpiryTime(DATE newValue);
    DATE GetFlagDueBy();
    void SetFlagDueBy(DATE newValue);
    CString GetFlagRequest();
    void SetFlagRequest(LPCTSTR lpszNewValue);
    long GetFlagStatus();
    void SetFlagStatus(long nNewValue);
    BOOL GetOriginatorDeliveryReportRequested();
    void SetOriginatorDeliveryReportRequested(BOOL bNewValue);
    DATE GetReceivedTime();
    void SetReceivedTime(DATE newValue);
    LPDISPATCH GetRecipients();
    BOOL GetReminderSet();
    void SetReminderSet(BOOL bNewValue);
    DATE GetReminderTime();
    void SetReminderTime(DATE newValue);
    LPDISPATCH GetReplyRecipients();
    LPDISPATCH GetSaveSentMessageFolder();
    void SetRefSaveSentMessageFolder(LPDISPATCH newValue);
    CString GetSenderName();
    BOOL GetSent();
    DATE GetSentOn();
    BOOL GetSubmitted();
    LPDISPATCH Forward();
    LPDISPATCH GetAssociatedAppointment(BOOL AddToCalendar);
    LPDISPATCH Reply();
    LPDISPATCH ReplyAll();
    void Send();
    LPDISPATCH GetLinks();
    long GetDownloadState();
    void ShowCategoriesDialog();
    LPDISPATCH GetItemProperties();
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    CString GetMeetingWorkspaceURL();
    long GetFlagIcon();
    void SetFlagIcon(long nNewValue);
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
    CString GetSenderEmailAddress();
    CString GetSenderEmailType();
};
/////////////////////////////////////////////////////////////////////////////
// _ContactItem wrapper class

class _ContactItem : public COleDispatchDriver
{
public:
    _ContactItem() {}		// Calls COleDispatchDriver default constructor
    _ContactItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _ContactItem(const _ContactItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    CString GetAccount();
    void SetAccount(LPCTSTR lpszNewValue);
    DATE GetAnniversary();
    void SetAnniversary(DATE newValue);
    CString GetAssistantName();
    void SetAssistantName(LPCTSTR lpszNewValue);
    CString GetAssistantTelephoneNumber();
    void SetAssistantTelephoneNumber(LPCTSTR lpszNewValue);
    DATE GetBirthday();
    void SetBirthday(DATE newValue);
    CString GetBusiness2TelephoneNumber();
    void SetBusiness2TelephoneNumber(LPCTSTR lpszNewValue);
    CString GetBusinessAddress();
    void SetBusinessAddress(LPCTSTR lpszNewValue);
    CString GetBusinessAddressCity();
    void SetBusinessAddressCity(LPCTSTR lpszNewValue);
    CString GetBusinessAddressCountry();
    void SetBusinessAddressCountry(LPCTSTR lpszNewValue);
    CString GetBusinessAddressPostalCode();
    void SetBusinessAddressPostalCode(LPCTSTR lpszNewValue);
    CString GetBusinessAddressPostOfficeBox();
    void SetBusinessAddressPostOfficeBox(LPCTSTR lpszNewValue);
    CString GetBusinessAddressState();
    void SetBusinessAddressState(LPCTSTR lpszNewValue);
    CString GetBusinessAddressStreet();
    void SetBusinessAddressStreet(LPCTSTR lpszNewValue);
    CString GetBusinessFaxNumber();
    void SetBusinessFaxNumber(LPCTSTR lpszNewValue);
    CString GetBusinessHomePage();
    void SetBusinessHomePage(LPCTSTR lpszNewValue);
    CString GetBusinessTelephoneNumber();
    void SetBusinessTelephoneNumber(LPCTSTR lpszNewValue);
    CString GetCallbackTelephoneNumber();
    void SetCallbackTelephoneNumber(LPCTSTR lpszNewValue);
    CString GetCarTelephoneNumber();
    void SetCarTelephoneNumber(LPCTSTR lpszNewValue);
    CString GetChildren();
    void SetChildren(LPCTSTR lpszNewValue);
    CString GetCompanyAndFullName();
    CString GetCompanyLastFirstNoSpace();
    CString GetCompanyLastFirstSpaceOnly();
    CString GetCompanyMainTelephoneNumber();
    void SetCompanyMainTelephoneNumber(LPCTSTR lpszNewValue);
    CString GetCompanyName();
    void SetCompanyName(LPCTSTR lpszNewValue);
    CString GetComputerNetworkName();
    void SetComputerNetworkName(LPCTSTR lpszNewValue);
    CString GetCustomerID();
    void SetCustomerID(LPCTSTR lpszNewValue);
    CString GetDepartment();
    void SetDepartment(LPCTSTR lpszNewValue);
    CString GetEmail1Address();
    void SetEmail1Address(LPCTSTR lpszNewValue);
    CString GetEmail1AddressType();
    void SetEmail1AddressType(LPCTSTR lpszNewValue);
    CString GetEmail1DisplayName();
    CString GetEmail1EntryID();
    CString GetEmail2Address();
    void SetEmail2Address(LPCTSTR lpszNewValue);
    CString GetEmail2AddressType();
    void SetEmail2AddressType(LPCTSTR lpszNewValue);
    CString GetEmail2DisplayName();
    CString GetEmail2EntryID();
    CString GetEmail3Address();
    void SetEmail3Address(LPCTSTR lpszNewValue);
    CString GetEmail3AddressType();
    void SetEmail3AddressType(LPCTSTR lpszNewValue);
    CString GetEmail3DisplayName();
    CString GetEmail3EntryID();
    CString GetFileAs();
    void SetFileAs(LPCTSTR lpszNewValue);
    CString GetFirstName();
    void SetFirstName(LPCTSTR lpszNewValue);
    CString GetFTPSite();
    void SetFTPSite(LPCTSTR lpszNewValue);
    CString GetFullName();
    void SetFullName(LPCTSTR lpszNewValue);
    CString GetFullNameAndCompany();
    long GetGender();
    void SetGender(long nNewValue);
    CString GetGovernmentIDNumber();
    void SetGovernmentIDNumber(LPCTSTR lpszNewValue);
    CString GetHobby();
    void SetHobby(LPCTSTR lpszNewValue);
    CString GetHome2TelephoneNumber();
    void SetHome2TelephoneNumber(LPCTSTR lpszNewValue);
    CString GetHomeAddress();
    void SetHomeAddress(LPCTSTR lpszNewValue);
    CString GetHomeAddressCity();
    void SetHomeAddressCity(LPCTSTR lpszNewValue);
    CString GetHomeAddressCountry();
    void SetHomeAddressCountry(LPCTSTR lpszNewValue);
    CString GetHomeAddressPostalCode();
    void SetHomeAddressPostalCode(LPCTSTR lpszNewValue);
    CString GetHomeAddressPostOfficeBox();
    void SetHomeAddressPostOfficeBox(LPCTSTR lpszNewValue);
    CString GetHomeAddressState();
    void SetHomeAddressState(LPCTSTR lpszNewValue);
    CString GetHomeAddressStreet();
    void SetHomeAddressStreet(LPCTSTR lpszNewValue);
    CString GetHomeFaxNumber();
    void SetHomeFaxNumber(LPCTSTR lpszNewValue);
    CString GetHomeTelephoneNumber();
    void SetHomeTelephoneNumber(LPCTSTR lpszNewValue);
    CString GetInitials();
    void SetInitials(LPCTSTR lpszNewValue);
    CString GetInternetFreeBusyAddress();
    void SetInternetFreeBusyAddress(LPCTSTR lpszNewValue);
    CString GetISDNNumber();
    void SetISDNNumber(LPCTSTR lpszNewValue);
    CString GetJobTitle();
    void SetJobTitle(LPCTSTR lpszNewValue);
    BOOL GetJournal();
    void SetJournal(BOOL bNewValue);
    CString GetLanguage();
    void SetLanguage(LPCTSTR lpszNewValue);
    CString GetLastFirstAndSuffix();
    CString GetLastFirstNoSpace();
    CString GetLastFirstNoSpaceCompany();
    CString GetLastFirstSpaceOnly();
    CString GetLastFirstSpaceOnlyCompany();
    CString GetLastName();
    void SetLastName(LPCTSTR lpszNewValue);
    CString GetLastNameAndFirstName();
    CString GetMailingAddress();
    void SetMailingAddress(LPCTSTR lpszNewValue);
    CString GetMailingAddressCity();
    void SetMailingAddressCity(LPCTSTR lpszNewValue);
    CString GetMailingAddressCountry();
    void SetMailingAddressCountry(LPCTSTR lpszNewValue);
    CString GetMailingAddressPostalCode();
    void SetMailingAddressPostalCode(LPCTSTR lpszNewValue);
    CString GetMailingAddressPostOfficeBox();
    void SetMailingAddressPostOfficeBox(LPCTSTR lpszNewValue);
    CString GetMailingAddressState();
    void SetMailingAddressState(LPCTSTR lpszNewValue);
    CString GetMailingAddressStreet();
    void SetMailingAddressStreet(LPCTSTR lpszNewValue);
    CString GetManagerName();
    void SetManagerName(LPCTSTR lpszNewValue);
    CString GetMiddleName();
    void SetMiddleName(LPCTSTR lpszNewValue);
    CString GetMobileTelephoneNumber();
    void SetMobileTelephoneNumber(LPCTSTR lpszNewValue);
    CString GetNetMeetingAlias();
    void SetNetMeetingAlias(LPCTSTR lpszNewValue);
    CString GetNetMeetingServer();
    void SetNetMeetingServer(LPCTSTR lpszNewValue);
    CString GetNickName();
    void SetNickName(LPCTSTR lpszNewValue);
    CString GetOfficeLocation();
    void SetOfficeLocation(LPCTSTR lpszNewValue);
    CString GetOrganizationalIDNumber();
    void SetOrganizationalIDNumber(LPCTSTR lpszNewValue);
    CString GetOtherAddress();
    void SetOtherAddress(LPCTSTR lpszNewValue);
    CString GetOtherAddressCity();
    void SetOtherAddressCity(LPCTSTR lpszNewValue);
    CString GetOtherAddressCountry();
    void SetOtherAddressCountry(LPCTSTR lpszNewValue);
    CString GetOtherAddressPostalCode();
    void SetOtherAddressPostalCode(LPCTSTR lpszNewValue);
    CString GetOtherAddressPostOfficeBox();
    void SetOtherAddressPostOfficeBox(LPCTSTR lpszNewValue);
    CString GetOtherAddressState();
    void SetOtherAddressState(LPCTSTR lpszNewValue);
    CString GetOtherAddressStreet();
    void SetOtherAddressStreet(LPCTSTR lpszNewValue);
    CString GetOtherFaxNumber();
    void SetOtherFaxNumber(LPCTSTR lpszNewValue);
    CString GetOtherTelephoneNumber();
    void SetOtherTelephoneNumber(LPCTSTR lpszNewValue);
    CString GetPagerNumber();
    void SetPagerNumber(LPCTSTR lpszNewValue);
    CString GetPersonalHomePage();
    void SetPersonalHomePage(LPCTSTR lpszNewValue);
    CString GetPrimaryTelephoneNumber();
    void SetPrimaryTelephoneNumber(LPCTSTR lpszNewValue);
    CString GetProfession();
    void SetProfession(LPCTSTR lpszNewValue);
    CString GetRadioTelephoneNumber();
    void SetRadioTelephoneNumber(LPCTSTR lpszNewValue);
    CString GetReferredBy();
    void SetReferredBy(LPCTSTR lpszNewValue);
    long GetSelectedMailingAddress();
    void SetSelectedMailingAddress(long nNewValue);
    CString GetSpouse();
    void SetSpouse(LPCTSTR lpszNewValue);
    CString GetSuffix();
    void SetSuffix(LPCTSTR lpszNewValue);
    CString GetTelexNumber();
    void SetTelexNumber(LPCTSTR lpszNewValue);
    CString GetTitle();
    void SetTitle(LPCTSTR lpszNewValue);
    CString GetTTYTDDTelephoneNumber();
    void SetTTYTDDTelephoneNumber(LPCTSTR lpszNewValue);
    CString GetUser1();
    void SetUser1(LPCTSTR lpszNewValue);
    CString GetUser2();
    void SetUser2(LPCTSTR lpszNewValue);
    CString GetUser3();
    void SetUser3(LPCTSTR lpszNewValue);
    CString GetUser4();
    void SetUser4(LPCTSTR lpszNewValue);
    CString GetUserCertificate();
    void SetUserCertificate(LPCTSTR lpszNewValue);
    CString GetWebPage();
    void SetWebPage(LPCTSTR lpszNewValue);
    CString GetYomiCompanyName();
    void SetYomiCompanyName(LPCTSTR lpszNewValue);
    CString GetYomiFirstName();
    void SetYomiFirstName(LPCTSTR lpszNewValue);
    CString GetYomiLastName();
    void SetYomiLastName(LPCTSTR lpszNewValue);
    LPDISPATCH ForwardAsVcard();
    LPDISPATCH GetLinks();
    LPDISPATCH GetItemProperties();
    CString GetLastFirstNoSpaceAndSuffix();
    long GetDownloadState();
    void ShowCategoriesDialog();
    CString GetIMAddress();
    void SetIMAddress(LPCTSTR lpszNewValue);
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    void SetEmail1DisplayName(LPCTSTR lpszNewValue);
    void SetEmail2DisplayName(LPCTSTR lpszNewValue);
    void SetEmail3DisplayName(LPCTSTR lpszNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
    void AddPicture(LPCTSTR Path);
    void RemovePicture();
    BOOL GetHasPicture();
};
/////////////////////////////////////////////////////////////////////////////
// _DistListItem wrapper class

class _DistListItem : public COleDispatchDriver
{
public:
    _DistListItem() {}		// Calls COleDispatchDriver default constructor
    _DistListItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _DistListItem(const _DistListItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    CString GetDLName();
    void SetDLName(LPCTSTR lpszNewValue);
    long GetMemberCount();
    LPDISPATCH GetLinks();
    void AddMembers(LPDISPATCH Recipients);
    void RemoveMembers(LPDISPATCH Recipients);
    LPDISPATCH GetMember(long Index);
    long GetDownloadState();
    void ShowCategoriesDialog();
    void AddMember(LPDISPATCH Recipient);
    void RemoveMember(LPDISPATCH Recipient);
    LPDISPATCH GetItemProperties();
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
};
/////////////////////////////////////////////////////////////////////////////
// _DocumentItem wrapper class

class _DocumentItem : public COleDispatchDriver
{
public:
    _DocumentItem() {}		// Calls COleDispatchDriver default constructor
    _DocumentItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _DocumentItem(const _DocumentItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    LPDISPATCH GetLinks();
    long GetDownloadState();
    void ShowCategoriesDialog();
    LPDISPATCH GetItemProperties();
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
};
/////////////////////////////////////////////////////////////////////////////
// ExplorersEvents wrapper class

class ExplorersEvents : public COleDispatchDriver
{
public:
    ExplorersEvents() {}		// Calls COleDispatchDriver default constructor
    ExplorersEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ExplorersEvents(const ExplorersEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void NewExplorer(LPDISPATCH Explorer);
};
/////////////////////////////////////////////////////////////////////////////
// FoldersEvents wrapper class

class FoldersEvents : public COleDispatchDriver
{
public:
    FoldersEvents() {}		// Calls COleDispatchDriver default constructor
    FoldersEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    FoldersEvents(const FoldersEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void FolderAdd(LPDISPATCH Folder);
    void FolderChange(LPDISPATCH Folder);
    void FolderRemove();
};
/////////////////////////////////////////////////////////////////////////////
// InspectorsEvents wrapper class

class InspectorsEvents : public COleDispatchDriver
{
public:
    InspectorsEvents() {}		// Calls COleDispatchDriver default constructor
    InspectorsEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    InspectorsEvents(const InspectorsEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void NewInspector(LPDISPATCH Inspector);
};
/////////////////////////////////////////////////////////////////////////////
// ItemsEvents wrapper class

class ItemsEvents : public COleDispatchDriver
{
public:
    ItemsEvents() {}		// Calls COleDispatchDriver default constructor
    ItemsEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ItemsEvents(const ItemsEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void ItemAdd(LPDISPATCH Item);
    void ItemChange(LPDISPATCH Item);
    void ItemRemove();
};
/////////////////////////////////////////////////////////////////////////////
// _JournalItem wrapper class

class _JournalItem : public COleDispatchDriver
{
public:
    _JournalItem() {}		// Calls COleDispatchDriver default constructor
    _JournalItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _JournalItem(const _JournalItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    CString GetContactNames();
    void SetContactNames(LPCTSTR lpszNewValue);
    BOOL GetDocPosted();
    void SetDocPosted(BOOL bNewValue);
    BOOL GetDocPrinted();
    void SetDocPrinted(BOOL bNewValue);
    BOOL GetDocRouted();
    void SetDocRouted(BOOL bNewValue);
    BOOL GetDocSaved();
    void SetDocSaved(BOOL bNewValue);
    long GetDuration();
    void SetDuration(long nNewValue);
    DATE GetEnd();
    void SetEnd(DATE newValue);
    CString GetType();
    void SetType(LPCTSTR lpszNewValue);
    LPDISPATCH GetRecipients();
    DATE GetStart();
    void SetStart(DATE newValue);
    LPDISPATCH Forward();
    LPDISPATCH Reply();
    LPDISPATCH ReplyAll();
    void StartTimer();
    void StopTimer();
    LPDISPATCH GetLinks();
    long GetDownloadState();
    void ShowCategoriesDialog();
    LPDISPATCH GetItemProperties();
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
};
/////////////////////////////////////////////////////////////////////////////
// NameSpaceEvents wrapper class

class NameSpaceEvents : public COleDispatchDriver
{
public:
    NameSpaceEvents() {}		// Calls COleDispatchDriver default constructor
    NameSpaceEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    NameSpaceEvents(const NameSpaceEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void OptionsPagesAdd(LPDISPATCH Pages, LPDISPATCH Folder);
};
/////////////////////////////////////////////////////////////////////////////
// _NoteItem wrapper class

class _NoteItem : public COleDispatchDriver
{
public:
    _NoteItem() {}		// Calls COleDispatchDriver default constructor
    _NoteItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _NoteItem(const _NoteItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    long GetColor();
    void SetColor(long nNewValue);
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetGetInspector();
    long GetHeight();
    void SetHeight(long nNewValue);
    DATE GetLastModificationTime();
    long GetLeft();
    void SetLeft(long nNewValue);
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    BOOL GetSaved();
    long GetSize();
    CString GetSubject();
    long GetTop();
    void SetTop(long nNewValue);
    long GetWidth();
    void SetWidth(long nNewValue);
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    LPDISPATCH GetLinks();
    long GetDownloadState();
    LPDISPATCH GetItemProperties();
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
};
/////////////////////////////////////////////////////////////////////////////
// OutlookBarGroup wrapper class

class OutlookBarGroup : public COleDispatchDriver
{
public:
    OutlookBarGroup() {}		// Calls COleDispatchDriver default constructor
    OutlookBarGroup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OutlookBarGroup(const OutlookBarGroup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    LPDISPATCH GetShortcuts();
    long GetViewType();
    void SetViewType(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// _OutlookBarShortcuts wrapper class

class _OutlookBarShortcuts : public COleDispatchDriver
{
public:
    _OutlookBarShortcuts() {}		// Calls COleDispatchDriver default constructor
    _OutlookBarShortcuts(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _OutlookBarShortcuts(const _OutlookBarShortcuts& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(const VARIANT& Target, LPCTSTR Name, const VARIANT& Index);
    void Remove(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// OutlookBarShortcut wrapper class

class OutlookBarShortcut : public COleDispatchDriver
{
public:
    OutlookBarShortcut() {}		// Calls COleDispatchDriver default constructor
    OutlookBarShortcut(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OutlookBarShortcut(const OutlookBarShortcut& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetTarget();
    void SetIcon(const VARIANT& Icon);
};
/////////////////////////////////////////////////////////////////////////////
// _OutlookBarGroups wrapper class

class _OutlookBarGroups : public COleDispatchDriver
{
public:
    _OutlookBarGroups() {}		// Calls COleDispatchDriver default constructor
    _OutlookBarGroups(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _OutlookBarGroups(const _OutlookBarGroups& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(LPCTSTR Name, const VARIANT& Index);
    void Remove(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// OutlookBarGroupsEvents wrapper class

class OutlookBarGroupsEvents : public COleDispatchDriver
{
public:
    OutlookBarGroupsEvents() {}		// Calls COleDispatchDriver default constructor
    OutlookBarGroupsEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OutlookBarGroupsEvents(const OutlookBarGroupsEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void GroupAdd(LPDISPATCH NewGroup);
    void BeforeGroupAdd(BOOL* Cancel);
    void BeforeGroupRemove(LPDISPATCH Group, BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// _OutlookBarPane wrapper class

class _OutlookBarPane : public COleDispatchDriver
{
public:
    _OutlookBarPane() {}		// Calls COleDispatchDriver default constructor
    _OutlookBarPane(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _OutlookBarPane(const _OutlookBarPane& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetContents();
    LPDISPATCH GetCurrentGroup();
    void SetRefCurrentGroup(LPDISPATCH newValue);
    CString GetName();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OutlookBarStorage wrapper class

class OutlookBarStorage : public COleDispatchDriver
{
public:
    OutlookBarStorage() {}		// Calls COleDispatchDriver default constructor
    OutlookBarStorage(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OutlookBarStorage(const OutlookBarStorage& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetGroups();
};
/////////////////////////////////////////////////////////////////////////////
// OutlookBarPaneEvents wrapper class

class OutlookBarPaneEvents : public COleDispatchDriver
{
public:
    OutlookBarPaneEvents() {}		// Calls COleDispatchDriver default constructor
    OutlookBarPaneEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OutlookBarPaneEvents(const OutlookBarPaneEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void BeforeNavigate(LPDISPATCH Shortcut, BOOL* Cancel);
    void BeforeGroupSwitch(LPDISPATCH ToGroup, BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// OutlookBarShortcutsEvents wrapper class

class OutlookBarShortcutsEvents : public COleDispatchDriver
{
public:
    OutlookBarShortcutsEvents() {}		// Calls COleDispatchDriver default constructor
    OutlookBarShortcutsEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OutlookBarShortcutsEvents(const OutlookBarShortcutsEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void ShortcutAdd(LPDISPATCH NewShortcut);
    void BeforeShortcutAdd(BOOL* Cancel);
    void BeforeShortcutRemove(LPDISPATCH Shortcut, BOOL* Cancel);
};
/////////////////////////////////////////////////////////////////////////////
// PropertyPageSite wrapper class

class PropertyPageSite : public COleDispatchDriver
{
public:
    PropertyPageSite() {}		// Calls COleDispatchDriver default constructor
    PropertyPageSite(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PropertyPageSite(const PropertyPageSite& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    void OnStatusChange();
};
/////////////////////////////////////////////////////////////////////////////
// Pages wrapper class

class Pages : public COleDispatchDriver
{
public:
    Pages() {}		// Calls COleDispatchDriver default constructor
    Pages(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Pages(const Pages& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(const VARIANT& Name);
    void Remove(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// _PostItem wrapper class

class _PostItem : public COleDispatchDriver
{
public:
    _PostItem() {}		// Calls COleDispatchDriver default constructor
    _PostItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _PostItem(const _PostItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    DATE GetExpiryTime();
    void SetExpiryTime(DATE newValue);
    CString GetHTMLBody();
    void SetHTMLBody(LPCTSTR lpszNewValue);
    DATE GetReceivedTime();
    CString GetSenderName();
    DATE GetSentOn();
    void ClearConversationIndex();
    LPDISPATCH Forward();
    void Post();
    LPDISPATCH Reply();
    LPDISPATCH GetLinks();
    LPDISPATCH GetItemProperties();
    long GetBodyFormat();
    void SetBodyFormat(long nNewValue);
    long GetDownloadState();
    void ShowCategoriesDialog();
    long GetInternetCodepage();
    void SetInternetCodepage(long nNewValue);
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
    CString GetSenderEmailAddress();
    CString GetSenderEmailType();
};
/////////////////////////////////////////////////////////////////////////////
// _RemoteItem wrapper class

class _RemoteItem : public COleDispatchDriver
{
public:
    _RemoteItem() {}		// Calls COleDispatchDriver default constructor
    _RemoteItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _RemoteItem(const _RemoteItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    BOOL GetHasAttachment();
    CString GetRemoteMessageClass();
    long GetTransferSize();
    long GetTransferTime();
    LPDISPATCH GetLinks();
    long GetDownloadState();
    void ShowCategoriesDialog();
    LPDISPATCH GetItemProperties();
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
};
/////////////////////////////////////////////////////////////////////////////
// _ReportItem wrapper class

class _ReportItem : public COleDispatchDriver
{
public:
    _ReportItem() {}		// Calls COleDispatchDriver default constructor
    _ReportItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _ReportItem(const _ReportItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    LPDISPATCH GetLinks();
    long GetDownloadState();
    void ShowCategoriesDialog();
    LPDISPATCH GetItemProperties();
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
};
/////////////////////////////////////////////////////////////////////////////
// _TaskItem wrapper class

class _TaskItem : public COleDispatchDriver
{
public:
    _TaskItem() {}		// Calls COleDispatchDriver default constructor
    _TaskItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _TaskItem(const _TaskItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    long GetActualWork();
    void SetActualWork(long nNewValue);
    CString GetCardData();
    void SetCardData(LPCTSTR lpszNewValue);
    BOOL GetComplete();
    void SetComplete(BOOL bNewValue);
    CString GetContactNames();
    void SetContactNames(LPCTSTR lpszNewValue);
    DATE GetDateCompleted();
    void SetDateCompleted(DATE newValue);
    long GetDelegationState();
    CString GetDelegator();
    DATE GetDueDate();
    void SetDueDate(DATE newValue);
    BOOL GetIsRecurring();
    long GetOrdinal();
    void SetOrdinal(long nNewValue);
    CString GetOwner();
    void SetOwner(LPCTSTR lpszNewValue);
    long GetOwnership();
    long GetPercentComplete();
    void SetPercentComplete(long nNewValue);
    LPDISPATCH GetRecipients();
    DATE GetReminderTime();
    void SetReminderTime(DATE newValue);
    BOOL GetReminderOverrideDefault();
    void SetReminderOverrideDefault(BOOL bNewValue);
    BOOL GetReminderPlaySound();
    void SetReminderPlaySound(BOOL bNewValue);
    BOOL GetReminderSet();
    void SetReminderSet(BOOL bNewValue);
    CString GetReminderSoundFile();
    void SetReminderSoundFile(LPCTSTR lpszNewValue);
    long GetResponseState();
    CString GetRole();
    void SetRole(LPCTSTR lpszNewValue);
    CString GetSchedulePlusPriority();
    void SetSchedulePlusPriority(LPCTSTR lpszNewValue);
    DATE GetStartDate();
    void SetStartDate(DATE newValue);
    long GetStatus();
    void SetStatus(long nNewValue);
    CString GetStatusOnCompletionRecipients();
    void SetStatusOnCompletionRecipients(LPCTSTR lpszNewValue);
    CString GetStatusUpdateRecipients();
    void SetStatusUpdateRecipients(LPCTSTR lpszNewValue);
    BOOL GetTeamTask();
    void SetTeamTask(BOOL bNewValue);
    long GetTotalWork();
    void SetTotalWork(long nNewValue);
    LPDISPATCH Assign();
    void CancelResponseState();
    void ClearRecurrencePattern();
    LPDISPATCH GetRecurrencePattern();
    void MarkComplete();
    LPDISPATCH Respond(long Response, const VARIANT& fNoUI, const VARIANT& fAdditionalTextDialog);
    void Send();
    BOOL SkipRecurrence();
    LPDISPATCH StatusReport();
    LPDISPATCH GetLinks();
    long GetDownloadState();
    void ShowCategoriesDialog();
    LPDISPATCH GetItemProperties();
    long GetInternetCodepage();
    void SetInternetCodepage(long nNewValue);
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
};
/////////////////////////////////////////////////////////////////////////////
// _TaskRequestAcceptItem wrapper class

class _TaskRequestAcceptItem : public COleDispatchDriver
{
public:
    _TaskRequestAcceptItem() {}		// Calls COleDispatchDriver default constructor
    _TaskRequestAcceptItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _TaskRequestAcceptItem(const _TaskRequestAcceptItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    LPDISPATCH GetAssociatedTask(BOOL AddToTaskList);
    LPDISPATCH GetLinks();
    long GetDownloadState();
    void ShowCategoriesDialog();
    LPDISPATCH GetItemProperties();
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
};
/////////////////////////////////////////////////////////////////////////////
// _TaskRequestDeclineItem wrapper class

class _TaskRequestDeclineItem : public COleDispatchDriver
{
public:
    _TaskRequestDeclineItem() {}		// Calls COleDispatchDriver default constructor
    _TaskRequestDeclineItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _TaskRequestDeclineItem(const _TaskRequestDeclineItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    LPDISPATCH GetAssociatedTask(BOOL AddToTaskList);
    LPDISPATCH GetLinks();
    long GetDownloadState();
    void ShowCategoriesDialog();
    LPDISPATCH GetItemProperties();
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
};
/////////////////////////////////////////////////////////////////////////////
// _TaskRequestItem wrapper class

class _TaskRequestItem : public COleDispatchDriver
{
public:
    _TaskRequestItem() {}		// Calls COleDispatchDriver default constructor
    _TaskRequestItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _TaskRequestItem(const _TaskRequestItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    LPDISPATCH GetAssociatedTask(BOOL AddToTaskList);
    LPDISPATCH GetLinks();
    long GetDownloadState();
    void ShowCategoriesDialog();
    LPDISPATCH GetItemProperties();
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
};
/////////////////////////////////////////////////////////////////////////////
// _TaskRequestUpdateItem wrapper class

class _TaskRequestUpdateItem : public COleDispatchDriver
{
public:
    _TaskRequestUpdateItem() {}		// Calls COleDispatchDriver default constructor
    _TaskRequestUpdateItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _TaskRequestUpdateItem(const _TaskRequestUpdateItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetClass();
    LPDISPATCH GetSession();
    LPDISPATCH GetParent();
    LPDISPATCH GetActions();
    LPDISPATCH GetAttachments();
    CString GetBillingInformation();
    void SetBillingInformation(LPCTSTR lpszNewValue);
    CString GetBody();
    void SetBody(LPCTSTR lpszNewValue);
    CString GetCategories();
    void SetCategories(LPCTSTR lpszNewValue);
    CString GetCompanies();
    void SetCompanies(LPCTSTR lpszNewValue);
    CString GetConversationIndex();
    CString GetConversationTopic();
    DATE GetCreationTime();
    CString GetEntryID();
    LPDISPATCH GetFormDescription();
    LPDISPATCH GetGetInspector();
    long GetImportance();
    void SetImportance(long nNewValue);
    DATE GetLastModificationTime();
    CString GetMessageClass();
    void SetMessageClass(LPCTSTR lpszNewValue);
    CString GetMileage();
    void SetMileage(LPCTSTR lpszNewValue);
    BOOL GetNoAging();
    void SetNoAging(BOOL bNewValue);
    long GetOutlookInternalVersion();
    CString GetOutlookVersion();
    BOOL GetSaved();
    long GetSensitivity();
    void SetSensitivity(long nNewValue);
    long GetSize();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    BOOL GetUnRead();
    void SetUnRead(BOOL bNewValue);
    LPDISPATCH GetUserProperties();
    void Close(long SaveMode);
    LPDISPATCH Copy();
    void Delete();
    void Display(const VARIANT& Modal);
    LPDISPATCH Move(LPDISPATCH DestFldr);
    void PrintOut();
    void Save();
    void SaveAs(LPCTSTR Path, const VARIANT& Type);
    LPDISPATCH GetAssociatedTask(BOOL AddToTaskList);
    LPDISPATCH GetLinks();
    long GetDownloadState();
    void ShowCategoriesDialog();
    LPDISPATCH GetItemProperties();
    long GetMarkForDownload();
    void SetMarkForDownload(long nNewValue);
    BOOL GetIsConflict();
    BOOL GetAutoResolvedWinner();
    LPDISPATCH GetConflicts();
};
/////////////////////////////////////////////////////////////////////////////
// ApplicationEvents_10 wrapper class

class ApplicationEvents_10 : public COleDispatchDriver
{
public:
    ApplicationEvents_10() {}		// Calls COleDispatchDriver default constructor
    ApplicationEvents_10(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ApplicationEvents_10(const ApplicationEvents_10& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    // method 'ItemSend' not emitted because of invalid return type or parameter type
    // method 'NewMail' not emitted because of invalid return type or parameter type
    // method 'Reminder' not emitted because of invalid return type or parameter type
    // method 'OptionsPagesAdd' not emitted because of invalid return type or parameter type
    // method 'Startup' not emitted because of invalid return type or parameter type
    // method 'Quit' not emitted because of invalid return type or parameter type
    void AdvancedSearchComplete(LPDISPATCH SearchObject);
    void AdvancedSearchStopped(LPDISPATCH SearchObject);
    void MAPILogonComplete();
};
/////////////////////////////////////////////////////////////////////////////
// ApplicationEvents_11 wrapper class

class ApplicationEvents_11 : public COleDispatchDriver
{
public:
    ApplicationEvents_11() {}		// Calls COleDispatchDriver default constructor
    ApplicationEvents_11(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ApplicationEvents_11(const ApplicationEvents_11& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    // method 'ItemSend' not emitted because of invalid return type or parameter type
    // method 'NewMail' not emitted because of invalid return type or parameter type
    // method 'Reminder' not emitted because of invalid return type or parameter type
    // method 'OptionsPagesAdd' not emitted because of invalid return type or parameter type
    // method 'Startup' not emitted because of invalid return type or parameter type
    // method 'Quit' not emitted because of invalid return type or parameter type
    // method 'AdvancedSearchComplete' not emitted because of invalid return type or parameter type
    // method 'AdvancedSearchStopped' not emitted because of invalid return type or parameter type
    // method 'MAPILogonComplete' not emitted because of invalid return type or parameter type
    void NewMailEx(LPCTSTR EntryIDCollection);
};
/////////////////////////////////////////////////////////////////////////////
// ResultsEvents wrapper class

class ResultsEvents : public COleDispatchDriver
{
public:
    ResultsEvents() {}		// Calls COleDispatchDriver default constructor
    ResultsEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ResultsEvents(const ResultsEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void ItemAdd(LPDISPATCH Item);
    void ItemChange(LPDISPATCH Item);
    void ItemRemove();
};
/////////////////////////////////////////////////////////////////////////////
// _ViewsEvents wrapper class

class _ViewsEvents : public COleDispatchDriver
{
public:
    _ViewsEvents() {}		// Calls COleDispatchDriver default constructor
    _ViewsEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _ViewsEvents(const _ViewsEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void ViewAdd(LPDISPATCH View);
    void ViewRemove(LPDISPATCH View);
};
/////////////////////////////////////////////////////////////////////////////
// ReminderCollectionEvents wrapper class

class ReminderCollectionEvents : public COleDispatchDriver
{
public:
    ReminderCollectionEvents() {}		// Calls COleDispatchDriver default constructor
    ReminderCollectionEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ReminderCollectionEvents(const ReminderCollectionEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void BeforeReminderShow(BOOL* Cancel);
    void ReminderAdd(LPDISPATCH ReminderObject);
    void ReminderChange(LPDISPATCH ReminderObject);
    void ReminderFire(LPDISPATCH ReminderObject);
    void ReminderRemove();
    void Snooze(LPDISPATCH ReminderObject);
};
/////////////////////////////////////////////////////////////////////////////
// _DRecipientControl wrapper class

class _DRecipientControl : public COleDispatchDriver
{
public:
    _DRecipientControl() {}		// Calls COleDispatchDriver default constructor
    _DRecipientControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _DRecipientControl(const _DRecipientControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    // method 'GetEnabled' not emitted because of invalid return type or parameter type
    // method 'SetEnabled' not emitted because of invalid return type or parameter type
    long GetBackColor();
    void SetBackColor(long nNewValue);
    long GetForeColor();
    void SetForeColor(long nNewValue);
    // method 'GetReadOnly' not emitted because of invalid return type or parameter type
    // method 'SetReadOnly' not emitted because of invalid return type or parameter type
    LPDISPATCH GetFont();
    void SetFont(LPDISPATCH newValue);
    long GetSpecialEffect();
    void SetSpecialEffect(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// _DRecipientControlEvents wrapper class

class _DRecipientControlEvents : public COleDispatchDriver
{
public:
    _DRecipientControlEvents() {}		// Calls COleDispatchDriver default constructor
    _DRecipientControlEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _DRecipientControlEvents(const _DRecipientControlEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// _DDocSiteControl wrapper class

class _DDocSiteControl : public COleDispatchDriver
{
public:
    _DDocSiteControl() {}		// Calls COleDispatchDriver default constructor
    _DDocSiteControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _DDocSiteControl(const _DDocSiteControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    // method 'GetReadOnly' not emitted because of invalid return type or parameter type
    // method 'SetReadOnly' not emitted because of invalid return type or parameter type
};
/////////////////////////////////////////////////////////////////////////////
// _DDocSiteControlEvents wrapper class

class _DDocSiteControlEvents : public COleDispatchDriver
{
public:
    _DDocSiteControlEvents() {}		// Calls COleDispatchDriver default constructor
    _DDocSiteControlEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _DDocSiteControlEvents(const _DDocSiteControlEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
};
