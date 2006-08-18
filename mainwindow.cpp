#include "mainwindow.h"
#include "kreversigame.h"
#include "kreversiscene.h"
#include "kreversiview.h"

#include <kaction.h>
#include <kapplication.h>
#include <kdebug.h>
#include <kicon.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kstdaction.h>
#include <kselectaction.h>

#include <QGraphicsView>

KReversiMainWindow::KReversiMainWindow(QWidget* parent)
    : KMainWindow(parent), m_scene(0), m_game(0), m_undoAct(0), m_hintAct(0)
{
    slotNewGame();
    // m_scene is created in slotNewGame();

    m_view = new KReversiView(m_scene, this);
    m_view->show();

    setupActions();
    setCentralWidget(m_view);
    setupGUI();
}

void KReversiMainWindow::setupActions()
{
    KAction *newGameAct = KStdAction::openNew(this, SLOT(slotNewGame()), actionCollection(), "new_game");
    KAction *quitAct = KStdAction::quit(this, SLOT(close()), actionCollection(), "quit");
    m_undoAct = KStdAction::undo( this, SLOT(slotUndo()), actionCollection(), "undo" );
    m_undoAct->setEnabled( false ); // nothing to undo at the start of the game
    m_hintAct = new KAction( KIcon("wizard"), i18n("Hint"), actionCollection(), "hint" );
    m_hintAct->setShortcut( Qt::Key_H );
    connect( m_hintAct, SIGNAL(triggered(bool)), m_scene, SLOT(slotHint()) );

    KSelectAction *bkgndAct = new KSelectAction(i18n("Choose background"), actionCollection(), "choose_bkgnd");
    connect(bkgndAct, SIGNAL(triggered(const QString&)), SLOT(slotBackgroundChanged(const QString&)));

    QStringList pixList = kapp->dirs()->findAllResources( "appdata", "pics/background/*.png", false, true );
    // let's find a name of files w/o extensions
    // FIXME dimsuz: this wont work with Windows separators...
    // But let's fix problems as they come (maybe will be some generalized solution in future)
    foreach( QString str, pixList )
    {
        int idx1 = str.lastIndexOf('/');
        int idx2 = str.lastIndexOf('.');
        bkgndAct->addAction(str.mid(idx1+1,idx2-idx1-1));
    }


    // FIXME dimsuz: this should come from KConfig!
    bkgndAct->setCurrentAction( "Hexagon" );
    slotBackgroundChanged("Hexagon");


    addAction(newGameAct);
    addAction(quitAct);
    addAction(m_undoAct);
    addAction(m_hintAct);
}

void KReversiMainWindow::slotBackgroundChanged( const QString& text )
{
    // FIXME dimsuz: I'm removing "&" from text here, because
    // there's a problem in KSelectAction ATM - text will contain a menu accell-ampersands
    // remove that .remove, after it is fixed
    QString file = text + ".png";
    file.remove('&');
    QPixmap pix( KStandardDirs::locate("appdata", QString("pics/background/") + file ) );
    if(!pix.isNull())
    {
        m_view->resetCachedContent();
        m_scene->setBackgroundPixmap( pix );
    }
}

void KReversiMainWindow::slotNewGame()
{
    delete m_game;
    m_game = new KReversiGame;
    connect( m_game, SIGNAL(moveFinished()), SLOT(slotMoveFinished()) );

    if(m_hintAct)
        m_hintAct->setEnabled( true );

    if(m_scene == 0) // if called first time
    {
        // FIXME dimsuz: if chips.png not found give error end exit
        m_scene = new KReversiScene(m_game, KStandardDirs::locate("appdata", "pics/chips.png"));
        connect( m_scene, SIGNAL(gameOver()), SLOT(slotGameOver()) );
    }
    else
    {
        m_scene->setGame( m_game );
    }
}

void KReversiMainWindow::slotGameOver()
{
    m_hintAct->setEnabled(false);
}

void KReversiMainWindow::slotMoveFinished()
{
    m_undoAct->setEnabled( m_game->canUndo() );    
}

void KReversiMainWindow::slotUndo()
{
    if( !m_scene->isBusy() )
    {
        // scene will automatically notice that it needs to update
        m_game->undo();
        m_undoAct->setEnabled( m_game->canUndo() );    
        // if the user hits undo after game is over
        // let's give him a chance to ask for a hint ;)
        m_hintAct->setEnabled( true );
    }
}

#include "mainwindow.moc"
