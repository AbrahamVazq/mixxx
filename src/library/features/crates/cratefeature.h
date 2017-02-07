#ifndef CRATEFEATURE_H
#define CRATEFEATURE_H

#include <QList>
#include <QPair>
#include <QAction>
#include <QVariant>
#include <QUrl>
#include <QIcon>
#include <QPersistentModelIndex>
#include <QPoint>
#include <QSet>
#include <QStackedWidget>

#include "library/libraryfeature.h"
#include "library/treeitemmodel.h"
#include "preferences/usersettings.h"
#include "track/track.h"

class TrackCollection;
class TreeItemModel;
class CrateTableModel;
class CrateDAO;

class CrateFeature : public LibraryFeature {
    Q_OBJECT
  public:
    CrateFeature(UserSettingsPointer pConfig,
                 Library* pLibrary,
                 QObject* parent,
                 TrackCollection* pTrackCollection);
    virtual ~CrateFeature();

    QVariant title() override;
    QString getIconPath() override;
    QString getSettingsName() const override;
    bool isSinglePane() const override;
    
    void onSearch(QString&) {}

    bool dragMoveAccept(QUrl url);
    bool dropAcceptChild(const QModelIndex& index, QList<QUrl> urls,
                         QObject* pSource);
    bool dragMoveAcceptChild(const QModelIndex& index, QUrl url);

    QWidget* createPaneWidget(KeyboardEventFilter* pKeyboard, int paneId) override;
    
    TreeItemModel* getChildModel();

  signals:
    void analyzeTracks(QList<TrackId>);

  public slots:
    void activate() override;
    void activateChild(const QModelIndex& index) override;
    void invalidateChild() override;
    void activateCrate(int crateId);
    void onRightClick(const QPoint& globalPos) override;
    void onRightClickChild(const QPoint& globalPos, const QModelIndex& index) override;

    void slotCreateCrate();
    void slotDeleteCrate();
    void slotRenameCrate();
    void slotDuplicateCrate();
    void slotAutoDjTrackSourceChanged();
    void slotToggleCrateLock();
    void slotImportPlaylist();
    void slotImportPlaylistFile(const QString &playlist_file);
    void slotCreateImportCrate();
    void slotExportPlaylist();
    // Copy all of the tracks in a crate to a new directory (like a thumbdrive).
    void slotExportTrackFiles();
    void slotAnalyzeCrate();
    void slotCrateTableChanged(int crateId);
    void slotCrateContentChanged(int crateId);
    void slotCrateTableRenamed(int crateId, QString a_strName);
    void htmlLinkClicked(const QUrl& link);

  private slots:
    void slotTrackSelected(TrackPointer pTrack);
    void slotResetSelectedTrack();

  private:
    QString getValidCrateName();
    QString getRootViewHtml() const;
    QModelIndex constructChildModel(int selected_id);
    void updateChildModel(int selected_id);
    void clearChildModel();
    void buildCrateList();
    int crateIdFromIndex(QModelIndex index);
    // Get the QModelIndex of a crate based on its id.  Returns QModelIndex()
    // on failure.
    QModelIndex indexFromCrateId(int crateId);
    
    QPointer<CrateTableModel> getTableModel(int paneId);
    void showBrowse(int paneId);
    void showTable(int paneId);

    TrackCollection* m_pTrackCollection;
    CrateDAO& m_crateDao;
    QAction* m_pCreateCrateAction;
    QAction* m_pDeleteCrateAction;
    QAction* m_pRenameCrateAction;
    QAction* m_pLockCrateAction;
    QAction* m_pDuplicateCrateAction;
    QAction* m_pAutoDjTrackSource;
    QAction* m_pImportPlaylistAction;
    QAction* m_pCreateImportPlaylistAction;
    QAction* m_pExportPlaylistAction;
    QAction* m_pExportTrackFilesAction;
    QAction* m_pAnalyzeCrateAction;
    QList<QPair<int, QString> > m_crateList;
    QHash<int, QPointer<CrateTableModel> > m_crateTableModel;
    CrateTableModel* m_pCrateTableModel;
    QPersistentModelIndex m_lastRightClickedIndex;
    TreeItemModel m_childModel;
    TrackPointer m_pSelectedTrack;
    QSet<int> m_cratesSelectedTrackIsIn;
    QHash<int, QPointer<QStackedWidget> > m_panes;
    QHash<int,int> m_idBrowse;
    QHash<int,int> m_idTable;
    QHash<int, QPersistentModelIndex> m_lastClickedIndex;
};

#endif /* CRATEFEATURE_H */