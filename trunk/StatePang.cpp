#include "StatePang.h"

#include <vector>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <sys/stat.h>
#ifdef PENJIN_SDL
#define GET_SCREEN( ) ( screen )
#else
#define GET_SCREEN( )
#endif
#ifndef _MAX_PATH
#define _MAX_PATH  128
#endif

//#define _PANG_DEBUG_MODE

// Put all my code in a name space as I create my own custom types like Sprite that conflicts with stuff elsewhere.
namespace PangMiniGame
{
    #ifdef _PANG_DEBUG_MODE
    #   define rconst
    #else
    #   define rconst const
    #endif

    // Dimensions of Pandora screen, could probably get these elsewhere?
    static rconst float  kSCREEN_WIDTH                           = 800.f;
    static rconst float  kSCREEN_HEIGHT                          = 480.f;

    // Where the ground/floor is in the level.
    static rconst float  kFLOOR                                  = 440.f;

    // Gravity, measured in, err.... pixels per second?
    static rconst float  kGRAVITY_INITIAL                        = 400.f;
    static float         kGRAVITY                                = kGRAVITY_INITIAL;

    // How high the balls should bounce.
    static rconst float  kMAX_HEIGHTS[ 5 ]                       = { 480, 415, 340, 255, 160 };

    // How fast the player can walk.
    static rconst float  kPLAYER_SPEED                           = 340.f;

    // How fast the spear travels once fired.
    static rconst float  kSPEAR_SPEED                            = 10.f;

    // How fast the ball travels horizontally
    static rconst float  kBALL_X_SPEED_INITIAL                   = 3.45f;
    static float         kBALL_X_SPEED                           = kBALL_X_SPEED_INITIAL;

    // How long spears stay attached to ceiling for.
    static rconst float  kSPEAR_HOLD_TIME                        = 0.1;

    // How long the spear draws for after hitting a ball.
    static rconst float kSPEAR_HOLD_TIME_AFTER_BALL_COLLISION    = 0.05f;

    static rconst int   kGOD_MODE                                = 0;

    static rconst float kBALL_SCALES[ 5 ]                       = { 1.f, 1.f, 1.f, 1.f, 1.f };

    static rconst float kPLAYER_SCALE                           = 1.f;

    static rconst float kSPEAR_SCALE                            = 1.f;

    // Debug - when set the game will freeze, handy for checking collision detection.
    static bool         g_Frozen                                = false;
    static bool         g_FreezeOnPlayerBallCollision           = false;
    static bool         g_FreezeOnBallSpearCollision            = false;

    // Animation details, the non filename stuff could be stored in a file...
    struct AnimationDetails
    {
        char *  Filename;
        int     NumFrames;
    };
    AnimationDetails                kImageBackground            = { "images/Pang/background.png", 1 };
    AnimationDetails                kImageBalls[ 5 ]            = { { "images/Pang/ball0.png", 1 },
                                                                    { "images/Pang/ball1.png", 1 },
                                                                    { "images/Pang/ball2.png", 1 },
                                                                    { "images/Pang/ball3.png", 1 },
                                                                    { "images/Pang/ball4.png", 1 },     };
    AnimationDetails                kImageClouds                = { "images/Pang/clouds_128x128.png", 1 };
    AnimationDetails                kImageCloudsBottom          = { "images/Pang/clouds_bottom_192x128.png", 1 };
    AnimationDetails                kImageCloudsMiddle1         = { "images/Pang/clouds_middle1_256x256.png", 1 };
    AnimationDetails                kImageCloudsMiddle2         = { "images/Pang/clouds_middle2_256x256.png", 1 };
    AnimationDetails                kImageCloudsTop             = { "images/Pang/clouds_top_256x256.png", 1 };
    AnimationDetails                kImageExplosion             = { "images/Pang/explosion.png", 2 };
    AnimationDetails                kImagePlayerIdle            = { "images/Pang/player.png", 1 };
    AnimationDetails                kImagePlayerWalkLeft        = { "images/Pang/player_walk_left.png", 6 };
    AnimationDetails                kImagePlayerWalkRight       = { "images/Pang/player_walk_right.png", 6 };
    AnimationDetails                kImageSpear                 = { "images/Pang/spear.png", 1 };

    class Controls
    {
    public:

        static void Update( SimpleJoy * joy );

        static bool IsUpHit( );
        static bool IsLeftPressed( );
        static bool IsRightPressed( );

    private:
        static unsigned m_IsLeftDown;
        static unsigned m_IsRightDown;
        static unsigned m_IsUpDown;
    };
    unsigned Controls::m_IsLeftDown     = 0;
    unsigned Controls::m_IsRightDown    = 0;
    unsigned Controls::m_IsUpDown       = 0;

    void Controls::Update( SimpleJoy * joy )
    {
        // Left.
        if ( joy->isLeft( ) )
        {
            if ( m_IsLeftDown == 0 )
                m_IsLeftDown = 1;
            else
                m_IsLeftDown = 2;
        }
        else
        {
            m_IsLeftDown = 0;
        }
        // Right.
        if ( joy->isRight( ) )
        {
            if ( m_IsRightDown == 0 )
                m_IsRightDown = 1;
            else
                m_IsRightDown = 2;
        }
        else
        {
            m_IsRightDown = 0;
        }
        // Up.
        if ( joy->isUp( ) || joy->isA())
        {
            if ( m_IsUpDown == 0 )
                m_IsUpDown = 1;
            else
                m_IsUpDown = 2;
        }
        else
        {
            m_IsUpDown = 0;
        }
    }
    bool Controls::IsLeftPressed( )
    {
        return m_IsLeftDown > 0;
    }
    bool Controls::IsRightPressed( )
    {
        return m_IsRightDown > 0;
    }
    bool Controls::IsUpHit( )
    {
        return m_IsUpDown == 1;
    }

    // Based on top of the Penjin sound class, only this one supports the same sound playing multiple times.
    class Sfx
    {
    public:

                    Sfx( );

        bool        Initialise( char const * const pFilename, int maxInstances = 1 );

        bool        Destroy( );

        bool        Play( );

    private:

        Sound **    m_ppSound;

        int         m_Instances;
    };

    Sfx::Sfx( )
    {
        m_ppSound = NULL;
    }

    bool Sfx::Initialise( char const * const pFilename, int maxInstances /* = 1 */ )
    {
        m_Instances = maxInstances;

        m_ppSound = new Sound * [ m_Instances ];

        for ( int i = 0; i < m_Instances; ++i )
        {
            m_ppSound[ i ] = new Sound;

            m_ppSound[ i ]->loadSound( pFilename );
        }

        return true;
    }

    bool Sfx::Destroy( )
    {
        if ( m_ppSound )
        {
            for ( int i = 0; i < m_Instances; ++i )
            {
                m_ppSound[ i ]->freeAll( );
                delete m_ppSound[ i ];
                m_ppSound[ i ] = NULL;
            }
            m_ppSound = NULL;
        }
    }

    bool Sfx::Play( )
    {
        if ( m_ppSound )
        {
            for ( int i = 0; i < m_Instances; ++i )
            {
                if ( ! m_ppSound[ i ]->isPlaying( ) )
                {
                    m_ppSound[ i ]->play( );
                    return true;
                }
            }
        }
        return false;
    }

    class SurfaceResourceManager
    {
    public:

                    SurfaceResourceManager( );

                    ~SurfaceResourceManager( );

        static bool Initialise( );

        static bool Destroy( );

        static bool LoadSurface( char const * const pFilename, SDL_Surface ** ppSurface, float alphaMultiplier = 1.f, float scale = 1.f );

        static bool FreeSurface( SDL_Surface * pSurface, float alphaMultiplier = 1.f );

    private:

        struct RefCountedSurface
        {
            RefCountedSurface( );

            bool operator == ( const RefCountedSurface & rhs );

            char            Filename[ _MAX_PATH ];
            SDL_Surface *   pSurface;
            int             RefCount;
            float           AlphaMultiplier;
            float           Scale;
        };

        static std::vector< RefCountedSurface >    m_RefCountedSurfaces;
    };
    SurfaceResourceManager::RefCountedSurface::RefCountedSurface( ) : pSurface( NULL ), RefCount( 0 ), AlphaMultiplier( 1.f ), Scale( 1.f )
    {
    }
    bool SurfaceResourceManager::RefCountedSurface::operator == ( const RefCountedSurface::RefCountedSurface & rhs )
    {
        if ( rhs.pSurface )
        {
            if ( pSurface == rhs.pSurface )
                return true;
            else
                return false;
        }
        else
        {
            if ( AlphaMultiplier == rhs.AlphaMultiplier && Scale == rhs.Scale )
            {
                return ( strcmp( Filename, rhs.Filename ) == 0 );
            }
            else
            {
                return false;
            }
        }
    }

    std::vector< SurfaceResourceManager::RefCountedSurface > SurfaceResourceManager::m_RefCountedSurfaces;

    SurfaceResourceManager::SurfaceResourceManager( )
    {
    }
    SurfaceResourceManager::~SurfaceResourceManager( )
    {
    }
    bool SurfaceResourceManager::Initialise( )
    {
        return true;
    }
    bool SurfaceResourceManager::Destroy( )
    {
        return true;
    }
    bool SurfaceResourceManager::LoadSurface( char const * const pFilename, SDL_Surface ** ppSurface, float alphaMultiplier /*= 1.f*/, float scale /*= 1.f*/ )
    {
        RefCountedSurface ToFind;
        strcpy( ToFind.Filename, pFilename );
        ToFind.AlphaMultiplier = alphaMultiplier;
        ToFind.Scale = scale;
        ToFind.pSurface = NULL;
        std::vector< RefCountedSurface >::iterator it = std::find( m_RefCountedSurfaces.begin( ), m_RefCountedSurfaces.end( ), ToFind );
        if ( it != m_RefCountedSurfaces.end( ) )
        {
            ( *ppSurface ) = it->pSurface;
            it->RefCount++;
            return true;
        }
        else
        {
            //The image that's loaded
            SDL_Surface* loadedImage = NULL;

            //Load the image
            loadedImage = IMG_Load( pFilename );

            //If the image loaded
            if( loadedImage != NULL )
            {
                if ( scale != 1.f )
                {
                    // Lock surface if required...
                    if( SDL_MUSTLOCK( loadedImage ) )
                        SDL_LockSurface( loadedImage );

                        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                        Uint32 rmask = 0xff000000;
                        Uint32 gmask = 0x00ff0000;
                        Uint32 bmask = 0x0000ff00;
                        Uint32 amask = 0x000000ff;
                        #else
                        Uint32 rmask = 0x000000ff;
                        Uint32 gmask = 0x0000ff00;
                        Uint32 bmask = 0x00ff0000;
                        Uint32 amask = 0xff000000;
                        #endif

                        SDL_Surface * newSurface = SDL_CreateRGBSurface(
                            loadedImage->flags,
                            loadedImage->w * scale,
                            loadedImage->h * scale,
                            32,
                            loadedImage->format->Rmask,
                            loadedImage->format->Gmask,
                            loadedImage->format->Bmask,
                            loadedImage->format->Amask );

                    // Lock surface if required...
                    if( SDL_MUSTLOCK( newSurface ) )
                        SDL_LockSurface( newSurface );

                    Uint32 * pixel = (Uint32*)newSurface->pixels;

                    unsigned char * newSurfacePixelData = ( unsigned char *) newSurface->pixels;
                    unsigned char * oldSurfacePixelData = ( unsigned char *) loadedImage->pixels;

                    for ( int y = 0; y < newSurface->h; ++y )
                    {
                        pixel = (Uint32*)&( newSurfacePixelData[ y * newSurface->pitch ] );
                        for ( int x = 0; x < newSurface->w; ++x )
                        {
                            int xToUse = floorf( x/scale );
                            int yToUse = floorf( y/scale );
                            pixel[ x ] = *(Uint32*)&oldSurfacePixelData[ yToUse * loadedImage->pitch + xToUse*(loadedImage->format->BitsPerPixel/8) ];
                        }
                    }

                    // Unlock surfaces if required...
                    if( SDL_MUSTLOCK( newSurface ) )
                        SDL_UnlockSurface( newSurface );

                    // Unlock surfaces if required...
                    if( SDL_MUSTLOCK( loadedImage ) )
                        SDL_UnlockSurface( loadedImage );

                        SDL_FreeSurface( loadedImage );

                        loadedImage = newSurface;
                }
                if ( alphaMultiplier >= 0.f && alphaMultiplier < 1.f )
                {
                    // Only support doing this on images with alpha channel!
                    assert( loadedImage->format->BytesPerPixel == 4 );

                    // Lock surface if required...
                    if( SDL_MUSTLOCK( loadedImage ) )
                        SDL_LockSurface( loadedImage );

                    unsigned char * a = (unsigned char *)loadedImage->pixels;
                    for ( int y = 0; y < loadedImage->h; ++y )
                    {
                        a = (unsigned char *)loadedImage->pixels + ( y * loadedImage->pitch ) + 3;
                        for ( int x = 0; x < loadedImage->w; ++x, a += 4 )
                        {
                            a[ 0 ] = (Uint8)( a[ 0 ] * alphaMultiplier );
                        }
                    }

                    // Unlock surfaces if required...
                    if( SDL_MUSTLOCK( loadedImage ) )
                        SDL_UnlockSurface( loadedImage );
                }

                RefCountedSurface refCountedSurface;
                refCountedSurface.AlphaMultiplier = alphaMultiplier;
                strcpy( refCountedSurface.Filename, pFilename );
                //Create an optimized surface
                refCountedSurface.pSurface = SDL_DisplayFormatAlpha( loadedImage );
                refCountedSurface.RefCount = 1;

                //Free the old surface
                SDL_FreeSurface( loadedImage );

                m_RefCountedSurfaces.push_back( refCountedSurface );

                ( *ppSurface ) = refCountedSurface.pSurface;
                return true;
            }
        }
        (*ppSurface) = NULL;
        return false;
    }
    bool SurfaceResourceManager::FreeSurface( SDL_Surface * pSurface, float alphaMultiplier /* = 1.f */ )
    {
        RefCountedSurface ToFind;
        ToFind.pSurface = pSurface;
        ToFind.AlphaMultiplier = alphaMultiplier;
        std::vector< RefCountedSurface >::iterator it = std::find( m_RefCountedSurfaces.begin( ), m_RefCountedSurfaces.end( ), ToFind );
        if ( it != m_RefCountedSurfaces.end( ) )
        {
            it->RefCount--;
            if ( it->RefCount == 0 )
            {
                SDL_FreeSurface( it->pSurface );
            }
            return true;
        }
        return false;
    }

    class Sprite
    {
    public:
        #ifdef PENJIN_SDL
        static bool         ClassInit( SDL_Surface * screen );
        #else
        static bool         ClassInit( );
        #endif

                            Sprite( char const * const pFilename, int numFrames = 1, float updateInterval = 0.05f, float alphaMultiplier = 1.f, bool looping = true, float scale = 1.f );

        bool                Update( float frameTime );

        bool                Render( );

        void                SetPosition( float x, float y );
        void                SetX( float x );
        void                SetY( float y );

        float               GetX( ) const;
        float               GetY( ) const;

        float               GetWidth( ) const;
        float               GetHeight( ) const;

        bool                IsAnimating( ) const;

        void                SetClippingRect( float x, float y, float w, float h );

        SDL_Surface *       GetSurfacePtr( );

        bool                HasCollidedWith( Sprite const * const pSpriteB );

    private:

        struct ClippingRect
        {
            float x, y;
            float w, h;
        };

        float                   m_PositionX, m_PositionY;
        int                     m_NumFrames;
        int                     m_CurrentFrame;
        bool                    m_IsLooping;
        bool                    m_IsAnimating;
        float                   m_UpdateInterval;
        float                   m_SinceLastUpdate;
        ClippingRect            m_ClippingRect;

        SDL_Surface *           m_pSurface;
        #ifdef PENJIN_SDL
        static SDL_Surface *    m_pScreen;
        #endif
    };
    #ifdef PENJIN_SDL
    SDL_Surface * Sprite::m_pScreen = NULL;
    #endif

    #ifdef PENJIN_SDL
    bool Sprite::ClassInit( SDL_Surface * screen )
    #else
    bool Sprite::ClassInit( )
    #endif
    {
        #ifdef PENJIN_SDL
        m_pScreen = screen;
        #endif

        return true;
    }
    Sprite::Sprite( char const * const pFilename, int numFrames /* = 1 */, float updateInterval /* = 0.05f */, float alphaMultiplier /* = 1.f */, bool looping /* = true */, float scale /* = 1.f */ )
    {
        m_PositionX         = 0;
        m_PositionY         = 0;
        m_NumFrames         = numFrames;
        m_UpdateInterval    = updateInterval;
        m_CurrentFrame      = 0;
        m_IsLooping         = looping;
        if ( m_NumFrames > 1 )
            m_IsAnimating   = true;
        m_SinceLastUpdate   = 0.f;

        //Load the image from the resouce manager (ref counted).
        SurfaceResourceManager::LoadSurface( pFilename, &m_pSurface, alphaMultiplier, scale );

        m_ClippingRect.x    = 0;
        m_ClippingRect.y    = 0;
        m_ClippingRect.w    = GetWidth( );
        m_ClippingRect.h    = GetHeight( );
    };
    void Sprite::SetPosition( float x, float y )
    {
        m_PositionX = x;
        m_PositionY = y;
    }
    void Sprite::SetX( float x )
    {
        m_PositionX = x;
    }
    void Sprite::SetY( float y )
    {
        m_PositionY = y;
    }
    float Sprite::GetX( ) const
    {
        return m_PositionX;
    }
    float Sprite::GetY( ) const
    {
        return m_PositionY;
    }
    float Sprite::GetWidth( ) const
    {
        return m_pSurface->w / (float)m_NumFrames;
    }
    float Sprite::GetHeight( ) const
    {
        return m_pSurface->h;
    }
    bool Sprite::IsAnimating( ) const
    {
        return m_IsAnimating;
    }
    void Sprite::SetClippingRect( float x, float y, float width, float height )
    {
        m_ClippingRect.x = x;
        m_ClippingRect.y = y;
        m_ClippingRect.w = width;
        m_ClippingRect.h = height;
    }
    SDL_Surface * Sprite::GetSurfacePtr( )
    {
        return m_pSurface;
    }
    bool Sprite::Update( float frameTime )
    {
        if ( m_IsAnimating )
        {
            m_SinceLastUpdate += frameTime;

            if ( m_SinceLastUpdate >= m_UpdateInterval )
            {
                m_SinceLastUpdate = 0;
                m_CurrentFrame++;
                if ( m_CurrentFrame == m_NumFrames )
                {
                    m_CurrentFrame = 0;
                    if ( m_IsLooping == false )
                    {
                        m_IsAnimating = false;
                    }
                }
            }
        }
        return true;
    }
    bool Sprite::Render( )
    {
        SDL_Rect src, dst;
        src.x = (Sint16)( m_ClippingRect.x + ( ( (float)m_CurrentFrame / m_NumFrames ) * m_pSurface->w ) );
        src.y = (Sint16)( m_ClippingRect.y );
        src.w = (Uint16)( m_ClippingRect.w );
        src.h = (Uint16)( m_ClippingRect.h );
        dst.x = (Sint16)( m_ClippingRect.x + (Sint16)m_PositionX );
        dst.y = (Sint16)( m_ClippingRect.y + (Sint16)m_PositionY );
        dst.w = (Uint16)( m_ClippingRect.w );
        dst.h = (Uint16)( m_ClippingRect.h );

        if ( dst.x < 0 )
        {
            src.x -= dst.x;
            dst.x = 0;
            if ( src.x >= src.w )
                return false;
        }

        SDL_BlitSurface( m_pSurface, &src, m_pScreen, &dst );

        return true;
    }
    bool Sprite::HasCollidedWith( Sprite const * const pSpriteB )
    {
        // Handle clip rect..
        int aXPos   = ( int )( GetX( ) + m_ClippingRect.x );
        int aYPos   = ( int )( GetY( ) + m_ClippingRect.y );

        int bXPos   = ( int )( pSpriteB->GetX( ) + pSpriteB->m_ClippingRect.x );
        int bYPos   = ( int )( pSpriteB->GetY( ) + pSpriteB->m_ClippingRect.y );

        int ax1     = ( int )( aXPos + m_ClippingRect.w - 1 );
        int ay1     = ( int )( aYPos + m_ClippingRect.h - 1 );

        int bx1     = ( int )( bXPos + pSpriteB->m_ClippingRect.w - 1 );
        int by1     = ( int )( bYPos + pSpriteB->m_ClippingRect.h - 1 );

        // Quick exit if there is no overlap.
        if ( ( bx1 < aXPos ) || ( ax1 < bXPos ) )
            return false;

        if ( ( by1 < aYPos ) || ( ay1 < bYPos ) )
            return false;

        // Calculate the start and end point of overlap.
        int xstart  = std::max< int >( aXPos, bXPos );
        int xend    = std::min< int >( ax1,bx1);

        int ystart  = std::max< int >( aYPos, bYPos );
        int yend    = std::min< int >( ay1,by1);

        // Only support alpha channel collision, could do more, but don't need to!
        assert( GetSurfacePtr( )->format->BytesPerPixel == 4 );
        assert( ((Sprite*)pSpriteB)->GetSurfacePtr( )->format->BytesPerPixel == 4 );

        // Lock surfaces if required...
        if( SDL_MUSTLOCK( GetSurfacePtr( ) ) )
            SDL_LockSurface( GetSurfacePtr( ) );

        if( SDL_MUSTLOCK( ((Sprite*)pSpriteB)->GetSurfacePtr( ) ) )
            SDL_LockSurface( ((Sprite*)pSpriteB)->GetSurfacePtr( ) );

        int spriteAPosX = ( int )GetX( );
        int spriteAPosY = ( int )GetY( );
        int spriteBPosX = ( int )pSpriteB->GetX( );
        int spriteBPosY = ( int )pSpriteB->GetY( );

        bool foundHit = false;
        unsigned char r1, g1, b1, a1;
        unsigned char r2, g2, b2, a2;
        for(int y = ystart ; y <= yend ; y += 1 )
        {
            for(int x = xstart ; x <= xend ; x += 1 )
            {
                SDL_GetRGBA(    *(Uint32*)((Uint8 *)GetSurfacePtr( )->pixels + (y-spriteAPosY) * GetSurfacePtr( )->pitch + (x-spriteAPosX + m_CurrentFrame * (int)GetWidth( )) * 4),
                                GetSurfacePtr( )->format,
                                &r1, &g1, &b1, &a1 );
                SDL_GetRGBA(    *(Uint32*)((Uint8 *)((Sprite*)pSpriteB)->GetSurfacePtr( )->pixels + (y-spriteBPosY) * ((Sprite*)pSpriteB)->GetSurfacePtr( )->pitch + (x-spriteBPosX + pSpriteB->m_CurrentFrame * (int)pSpriteB->GetWidth( )) * 4),
                                ((Sprite*)pSpriteB)->GetSurfacePtr( )->format,
                                &r2, &g2, &b2, &a2 );

                if ( a1 && a2 )
                {
                    foundHit = true;
                    break;
                }
            }
            if ( foundHit )
            {
                break;
            }
        }

        // Unlock surfaces if required...
        if( SDL_MUSTLOCK( GetSurfacePtr( ) ) )
            SDL_UnlockSurface(GetSurfacePtr( ) );

        if( SDL_MUSTLOCK( ((Sprite*)pSpriteB)->GetSurfacePtr( ) ) )
            SDL_UnlockSurface( ((Sprite*)pSpriteB)->GetSurfacePtr( ) );

        if ( foundHit )
            return true;
        else
            return false;
    }

    // --------------------------------------------------------------
    // BackgroundLayer.
    // --------------------------------------------------------------
    class BackgroundLayer
    {
    public:

        BackgroundLayer( );
        ~BackgroundLayer( );

        bool    Initialise( char const * const pGraphicFilename, float yPosition, bool isTiled, float speed );

        bool    Destroy( );

        bool    Update( float frameTime );

        bool    Render( float frameTime );

    private:

        Sprite *    m_pSprite[ 11 ];
        float       m_Speed;
        bool        m_IsTiled;
        float       m_YPosition;
        float       m_FirstTileXPosition;
    };

    BackgroundLayer::BackgroundLayer( )
    {
        memset( m_pSprite, 0, sizeof( Sprite* ) * 11 );
        m_IsTiled   = false;
        m_Speed     = 0.f;
    }
    BackgroundLayer::~BackgroundLayer( )
    {
        Destroy( );
    }
    bool BackgroundLayer::Initialise( char const * const pGraphicFilename, float yPosition, bool isTiled, float speed )
    {
        for ( int i = 0; i < 11; ++i )
        {
            m_pSprite[ i ] = new Sprite( pGraphicFilename, 1, 0.05f, ( float )( i ) / 10.f );
        }
        m_IsTiled   = isTiled;
        m_Speed     = speed;
        m_YPosition = yPosition;
        m_FirstTileXPosition = Random::nextFloat(0,1) * -m_pSprite[ 0 ]->GetWidth( );

        return true;
    }
    bool BackgroundLayer::Destroy( )
    {
        for ( int i = 0; i < 11; ++i )
        {
            if ( m_pSprite[ i ] )
            {
                delete m_pSprite[ i ];
                m_pSprite[ i ] = NULL;
            }
        }
        return true;
    }
    bool BackgroundLayer::Render( float frameTime )
    {
        float gap = 0.f;
        if ( m_IsTiled )
        {
            gap = -1.f;
        }
        else
        {
            gap = 200.f;
        }
        for ( float xPosToRender = m_FirstTileXPosition; xPosToRender <= kSCREEN_WIDTH; xPosToRender += m_pSprite[ 0 ]->GetWidth( ) + gap )
        {
            int xPos = ( int )floorf( xPosToRender );
            float scaler1 = 1.f - ( xPosToRender - ( float ) xPos );
            if ( scaler1 > 1.f )
                scaler1 = 1.f;
            if ( scaler1 < 0.f )
                scaler1 = 0.f;

            int ix1 = (int)( scaler1 * 10 );
            int ix2 = 10 - ix1;

            m_pSprite[ ix1 ]->SetPosition( xPos - 1, m_YPosition );
            m_pSprite[ ix1 ]->Render( );
            m_pSprite[  10 ]->SetPosition( xPos + 0, m_YPosition );
            m_pSprite[  10 ]->Render( );
            m_pSprite[ ix2 ]->SetPosition( xPos + 1, m_YPosition );
            m_pSprite[ ix2 ]->Render( );
        }
        return true;
    }
    bool BackgroundLayer::Update( float frameTime )
    {
        m_FirstTileXPosition += m_Speed * frameTime;
        if ( m_FirstTileXPosition > 0 )
        {
            m_FirstTileXPosition -= m_IsTiled ? ( m_pSprite[ 0 ]->GetWidth( ) - 1 ) : ( m_pSprite[ 0 ]->GetWidth( ) + 200.f );
        }
        return true;
    }
    // --------------------------------------------------------------
    // BackgroundLayaerManager.
    // --------------------------------------------------------------
    class BackgroundLayerManager
    {
    public:

                BackgroundLayerManager( );
                ~BackgroundLayerManager( );

        bool    Initialise( );

        bool    Destroy( );

        bool    AddLayer( char const * const pGraphicFilename, float yPosition, bool isTiled, float speed );

        bool    Update( float frameTime );

        bool    Render( float frameTime );

    private:

        std::vector< BackgroundLayer* > m_BackgroundLayers;

    };

    BackgroundLayerManager::BackgroundLayerManager( )
    {
    }
    BackgroundLayerManager::~BackgroundLayerManager( )
    {
    }
    bool BackgroundLayerManager::Initialise( )
    {
        return true;
    }
    bool BackgroundLayerManager::Destroy( )
    {
        for ( std::vector< BackgroundLayer * >::iterator it = m_BackgroundLayers.begin( ); it != m_BackgroundLayers.end( ); ++it )
        {
            BackgroundLayer *pBackgroundLayer = *it;
            pBackgroundLayer->Destroy( );
            delete pBackgroundLayer;
        }
        return true;
    }
    bool BackgroundLayerManager::AddLayer( char const * const pGraphicFilename, float yPosition, bool isTiled, float speed )
    {
        BackgroundLayer *pToAdd = new BackgroundLayer( );
        pToAdd->Initialise( pGraphicFilename, yPosition, isTiled, speed );
        m_BackgroundLayers.push_back( pToAdd );
        return true;
    }
    bool BackgroundLayerManager::Update( float frameTime )
    {
        for ( std::vector< BackgroundLayer * >::iterator it = m_BackgroundLayers.begin( ); it != m_BackgroundLayers.end( ); ++it )
        {
            BackgroundLayer *pBackgroundLayer = *it;
            pBackgroundLayer->Update( frameTime );
        }
        return true;
    }
    bool BackgroundLayerManager::Render( float frameTime )
    {
        for ( std::vector< BackgroundLayer * >::iterator it = m_BackgroundLayers.begin( ); it != m_BackgroundLayers.end( ); ++it )
        {
            BackgroundLayer *pBackgroundLayer = *it;
            pBackgroundLayer->Render( frameTime );
        }
        return true;
    }

    // --------------------------------------------------------------
    // Ball.
    // --------------------------------------------------------------
    class Ball
    {
    public:

        friend class BallManager;
        friend class PangGame;

        Ball( float posX, float posY, int size, int direction );

        Ball( const Ball & rhs );

        ~Ball( );

    private:

        enum Direction
        {
            kDirectionLeft  = -1,
            kDirectionRight = 1
        };

        int         m_Size;
        float       m_SpeedX, m_SpeedY;

        Sprite *    m_pSprite;
    };

    Ball::Ball( float posX, float posY, int size, int direction )
    {
        m_pSprite = new Sprite( kImageBalls[ size ].Filename, kImageBalls[ size ].NumFrames, 0.f, 1.f, false, kBALL_SCALES[ size ] );

        posX -= 0.5f * m_pSprite->GetWidth( );
        posY -= 0.5f * m_pSprite->GetHeight( );

        m_Size = size;
        m_SpeedY = 0.f;
        m_SpeedX = direction == kDirectionRight ? kBALL_X_SPEED : -kBALL_X_SPEED;

        float peak = posY - ( kSCREEN_HEIGHT - kMAX_HEIGHTS[ m_Size ] );
        if ( peak < 0 )
            m_SpeedY = 0;
        else
            m_SpeedY = -sqrtf( peak * 2.f * kGRAVITY );
        if ( m_SpeedY > 0 )
            m_SpeedY = 0;

        if ( m_SpeedY > -200.f )
            m_SpeedY = -200.f;

        m_pSprite->SetPosition( posX, posY );
    }
    Ball::Ball( const Ball & rhs )
    {
        m_Size = rhs.m_Size;
        m_SpeedX = rhs.m_SpeedX;
        m_SpeedY = rhs.m_SpeedY;
        m_pSprite = rhs.m_pSprite;
    }
    Ball::~Ball( )
    {
        if ( m_pSprite )
        {
            delete m_pSprite;
            m_pSprite = NULL;
        }
    }
    // --------------------------------------------------------------

    // --------------------------------------------------------------
    // BallManager.
    // --------------------------------------------------------------
    class BallManager
    {
        friend class PangGame;

    public:

        bool Initialise( );

        bool Destroy( );

        bool UpdateAll( float frameTime );

        bool RenderAll( );

        bool Add( float posX, float posY, int size, int direction );

        bool Add( Ball * pBall );

    private:

        std::vector< Ball* >  m_Balls;
    };
    bool BallManager::Initialise( )
    {
        return true;
    }
    bool BallManager::Destroy( )
    {
        return true;
    }
    bool BallManager::UpdateAll( float frameTime )
    {
        for ( std::vector< Ball* >::iterator ballIt = m_Balls.begin( ); ballIt != m_Balls.end( ); ++ballIt )
        {
            Ball * pBall = *ballIt;

            // Update X position/speed.
            pBall->m_pSprite->SetX( pBall->m_pSprite->GetX( ) + pBall->m_SpeedX );
            if ( pBall->m_pSprite->GetX( ) + pBall->m_pSprite->GetWidth( ) > kSCREEN_WIDTH )
            {
                pBall->m_pSprite->SetX( kSCREEN_WIDTH - ( pBall->m_pSprite->GetWidth( ) ) );
                pBall->m_SpeedX = -pBall->m_SpeedX;
            }
            else if ( pBall->m_pSprite->GetX( ) < 0.f )
            {
                pBall->m_pSprite->SetX( 0.f );
                pBall->m_SpeedX = -pBall->m_SpeedX;
            }

            // Update Y position/speed.
            pBall->m_SpeedY += kGRAVITY * frameTime;
            pBall->m_pSprite->SetY( pBall->m_pSprite->GetY( ) + pBall->m_SpeedY * frameTime );
            if ( pBall->m_pSprite->GetY( ) + pBall->m_pSprite->GetHeight( ) > kFLOOR )
            {
                pBall->m_pSprite->SetY( kFLOOR - pBall->m_pSprite->GetHeight( ) );
                //pBall->m_SpeedY = -25.f * kGRAVITY;
                float peak =  pBall->m_pSprite->GetY( ) - ( kSCREEN_HEIGHT - kMAX_HEIGHTS[ pBall->m_Size ] );
                pBall->m_SpeedY = -sqrtf( peak * 2.f * kGRAVITY );
            }
        }
        return true;
    }
    bool BallManager::RenderAll( )
    {
        for ( std::vector< Ball* >::iterator ballIt = m_Balls.begin( ); ballIt != m_Balls.end( ); ++ballIt )
        {
            Ball * pBall = *ballIt;

            pBall->m_pSprite->Render( );
        }
        return true;
    }
    bool BallManager::Add( float posX, float posY, int size, int direction )
    {
        Ball *pBall = new Ball( posX, posY, size, direction );
        m_Balls.push_back( pBall );
        return true;
    }
    bool BallManager::Add( Ball * pBall )
    {
        m_Balls.push_back( pBall );
        return true;
    }
    // --------------------------------------------------------------

    // --------------------------------------------------------------
    // Spear.
    // --------------------------------------------------------------
    class Spear
    {
        friend class SpearManager;
        friend class PangGame;

    public:

        Spear( float startX, float startY );

        ~Spear( );

    private:

        enum State
        {
            kState_Active,
            kState_HitBall
        };
        State       m_State;
        float       m_HitTime;
        float       m_BottomY;
        float       m_HoldTime;
        Sprite *    m_pSprite;
    };
    Spear::Spear( float startX, float startY )
    {
        m_State = kState_Active;
        m_HitTime = -1.f;
        m_BottomY = kFLOOR;
        m_pSprite = new Sprite( kImageSpear.Filename, kImageSpear.NumFrames, 0.05f, 1.f, true, kSPEAR_SCALE );
        m_pSprite->SetPosition( startX - 0.5f * m_pSprite->GetWidth( ), startY );
        m_pSprite->SetClippingRect( 0, 0, m_pSprite->GetWidth( ), m_BottomY - m_pSprite->GetY( ) );
        m_HoldTime  = -1.f;
    }
    Spear::~Spear( )
    {
        if ( m_pSprite )
        {
            delete m_pSprite;
            m_pSprite = NULL;
        }
    }
    // --------------------------------------------------------------

    // --------------------------------------------------------------
    // SpearManager.
    // --------------------------------------------------------------
    class SpearManager
    {
        friend class PangGame;

    public:

        bool Initialise( );

        bool Destroy( );

        bool UpdateAll( float frameTime );

        bool RenderAll( );

        bool Add( float posX, float posY );

        bool CanAddSpear( );

    private:

        std::vector< Spear* >    m_Spears;

    };
    bool SpearManager::CanAddSpear( )
    {
        return m_Spears.size( ) < 2;
    }
    bool SpearManager::Initialise( )
    {
        return true;
    }
    bool SpearManager::Destroy( )
    {
        return true;
    }
    bool SpearManager::UpdateAll( float frameTime )
    {
        for ( std::vector< Spear* >::iterator spearIt = m_Spears.begin( ); spearIt != m_Spears.end( ); )
        {
            Spear * pSpear = *spearIt;

            if ( pSpear->m_HoldTime >= 0 )
            {
                pSpear->m_HoldTime -= frameTime;
                if ( pSpear->m_HoldTime <= 0 )
                {
                    spearIt = m_Spears.erase( spearIt );
                    continue;
                }
            }
            else
            {
                if ( pSpear->m_State == Spear::kState_Active )
                {
                    pSpear->m_pSprite->SetY( pSpear->m_pSprite->GetY( ) - kSPEAR_SPEED );
                    if ( pSpear->m_pSprite->GetY( ) < 0.f )
                    {
                        pSpear->m_HoldTime = kSPEAR_HOLD_TIME;
                        pSpear->m_pSprite->SetY( 0.f );
                    }
                }
                else if ( pSpear->m_State == Spear::kState_HitBall )
                {
                    pSpear->m_HitTime -= frameTime;

                    if ( pSpear->m_HitTime <= 0.f )
                    {
                        delete *spearIt;
                        spearIt = m_Spears.erase( spearIt );
                        continue;
                    }
                }
            }
            spearIt++;
        }
        return true;
    }
    bool SpearManager::RenderAll( )
    {
        for ( std::vector< Spear* >::iterator spearIt = m_Spears.begin( ); spearIt != m_Spears.end( ); ++spearIt )
        {
            Spear * pSpear = *spearIt;

            // Update sprite clipping box.
            pSpear->m_pSprite->SetClippingRect( 0, 0, pSpear->m_pSprite->GetWidth( ), pSpear->m_BottomY - pSpear->m_pSprite->GetY( ) );

            pSpear->m_pSprite->Render( );
        }
        return true;
    }
    bool SpearManager::Add( float posX, float posY )
    {
        m_Spears.push_back( new Spear( posX, posY ) );
        return true;
    }
    // --------------------------------------------------------------

    // --------------------------------------------------------------
    // Explosion.
    // --------------------------------------------------------------
    class Explosion
    {
        friend class ExplosionManager;

    public:

                Explosion( float posX, float posY );

                ~Explosion( );

        bool    Update( float frameTime );

        bool    Render( );

    private:

        Sprite *    m_pSprite;
    };
    Explosion::Explosion( float posX, float posY )
    {
        m_pSprite = new Sprite( kImageExplosion.Filename, kImageExplosion.NumFrames, 0.3f, 1.f, false );
        m_pSprite->SetPosition( posX - 0.5f * m_pSprite->GetWidth( ), posY - 0.5f * m_pSprite->GetHeight( ) );
    }
    Explosion::~Explosion( )
    {
        if ( m_pSprite )
        {
            delete m_pSprite;
            m_pSprite = NULL;
        }
    }
    bool Explosion::Update( float frameTime )
    {
        return m_pSprite->Update( frameTime );
    }
    bool Explosion::Render( )
    {
        return m_pSprite->Render( );
    }

    // --------------------------------------------------------------

    // --------------------------------------------------------------
    // ExplosionManager.
    // --------------------------------------------------------------
    class ExplosionManager
    {
    public:

        bool    Add( float posX, float posY );

        bool    UpdateAll( float frameTime );

        bool    RenderAll( );

    private:

        std::vector< Explosion * >  m_Explosions;
    };
    bool ExplosionManager::Add( float posX, float posY )
    {
        m_Explosions.push_back( new Explosion( posX, posY ) );
        return true;
    }
    bool ExplosionManager::UpdateAll( float frameTime )
    {
        for ( std::vector< Explosion * >::iterator explosionIt = m_Explosions.begin( ); explosionIt != m_Explosions.end( ); )
        {
            Explosion * pExplosion = *explosionIt;
            pExplosion->Update( frameTime );
            if ( pExplosion->m_pSprite->IsAnimating( ) == false )
            {
                delete pExplosion;
                explosionIt = m_Explosions.erase( explosionIt );
            }
            else
            {
                ++explosionIt;
            }
        }
        return true;
    }
    bool ExplosionManager::RenderAll( )
    {
        for ( std::vector< Explosion * >::iterator explosionIt = m_Explosions.begin( ); explosionIt != m_Explosions.end( ); ++explosionIt )
        {
            Explosion * pExplosion = *explosionIt;
            pExplosion->Render( );
        }
        return true;
    }
    // --------------------------------------------------------------

    // --------------------------------------------------------------
    // PangGame.
    // --------------------------------------------------------------
    class PangGame
    {
    public:

                PangGame( );
                ~PangGame( );

        bool    Initialise( );

        bool    Destroy( );

        bool    StartLevel( int levelNumber );

        bool    Update( SimpleJoy * input );

        #ifdef PENJIN_SDL
        bool Render(SDL_Surface *screen );
        #else
        bool Render( float frameTime );
        #endif

        bool            IsLevelSuccessful( );
        bool            IsLevelFailed( );

    private:

        enum kPlayerAnim
        {
            kPlayerAnimWalkLeft,
            kPlayerAnimStand,
            kPlayerAnimWalkRight,
            kPlayerAnimNum
        };

        void            SetLevelComplete( bool successful );

        bool            ReadSettings( char const * const configFile = "scripts/Pang/config.ini" );

        time_t                      m_SettingsFileModifiedTime;

        BallManager *               m_pBallManager;
        SpearManager *              m_pSpearManager;
        ExplosionManager *          m_pExplosionManager;
        BackgroundLayerManager *    m_pBackgroundLayerManager;

        Sprite *                    m_pBackground;
        Sprite *                    m_pPlayer[ kPlayerAnimNum ];
        Sprite **                   m_ppPlayerCurrent;

        float                       m_TimeLastFrame;

        float                       m_FrameTime;

        bool                        m_LevelFailed;
        bool                        m_LevelSuccessful;

        // Define the dynamic resources that might be deleted during game play, but shouldn't be unloaded...
        enum LevelResourceImage
        {
            kLevelResourceImage_Ball0,
            kLevelResourceImage_Ball1,
            kLevelResourceImage_Ball2,
            kLevelResourceImage_Ball3,
            kLevelResourceImage_Ball4,
            kLevelResourceImage_Explosion,
            kLevelResourceImage_Spear,

            kLevelResourceImage_Num
        };
        Sprite *    m_pSprites[ kLevelResourceImage_Num ];

        enum SoundResource
        {
            kSoundResource_BallSpearHit,
            kSoundResource_Shoot,

            kSoundResource_MAX
        };

        Sfx         m_Sfx[ kSoundResource_MAX ];

        Music *     m_pMusic;

        int         m_NumFramesElapsedSinceLevelStart;
    };
    PangGame::PangGame( )
    {
        m_pBallManager                      = NULL;
        m_pSpearManager                     = NULL;
        m_pExplosionManager                 = NULL;
        m_pBackgroundLayerManager           = NULL;
        m_pBackground                       = NULL;
        m_pPlayer[ kPlayerAnimWalkLeft ]    = NULL;
        m_pPlayer[ kPlayerAnimStand ]       = NULL;
        m_pPlayer[ kPlayerAnimWalkRight ]   = NULL;
        m_ppPlayerCurrent                   = &m_pPlayer[ kPlayerAnimStand ];
        m_TimeLastFrame                     = 0;
        m_FrameTime                         = 0;
        m_TimeLastFrame                     = 0;
        m_FrameTime                         = 0;
        m_LevelFailed                       = false;
        m_LevelSuccessful                   = false;
        m_SettingsFileModifiedTime          = -1;
        m_pMusic                            = NULL;
        m_NumFramesElapsedSinceLevelStart   = 0;
    }
    PangGame::~PangGame( )
    {
        Destroy( );
    }
    bool PangGame::ReadSettings( char const * const configFile /* = "scripts/Pang/config.ini" */ )
    {
        #if !defined(_PANG_DEBUG_MODE)
        return true;
        #else
        struct stat fileStat;
        if ( stat( configFile, &fileStat ) )
            return false;
        if ( m_SettingsFileModifiedTime == fileStat.st_mtime )
            return true;
        else
            m_SettingsFileModifiedTime = fileStat.st_mtime;

        FILE * fh = fopen( configFile, "r" );
        if ( !fh )
            return false;

        size_t fileSize;

        fseek( fh, 0, SEEK_END );

        fileSize = ftell( fh );

        fseek( fh, 0, SEEK_SET );

        char * pBuffer = new char[ fileSize ];

        fread( pBuffer, fileSize, 1, fh );

        fclose( fh );

        char    line[ 1024 ];
        int     linePos = 0;

        bool    ballScalesThatHaveChanged[ 5 ] = { false, false, false, false, false };
        bool    playerScaleChanged = false;
        bool    spearScaleChanged = false;

        for ( size_t i = 0; i < fileSize; ++i )
        {
            if ( pBuffer[ i ] != '\n' && pBuffer[ i ] != '\r' && pBuffer[ i ] != '\0' )
            {
                line[ linePos++ ] = pBuffer[ i ];
            }
            else
            {
                if ( linePos > 0 )
                {
                    line[ linePos ] = '\0';

                    char * key          = NULL;
                    char * value        = NULL;
                    bool valueIsFloat   = true;
                    for ( size_t j = 0; j < linePos; ++j )
                    {
                        if ( line[ j ] == '=' )
                        {
                            if ( j == 0 )
                                break;

                            if ( linePos - j == 1 )
                                break;

                            line[ j ] = '\0';
                            key = &line[ 0 ];
                            value = &line[ j + 1 ];

                            if ( strchr( value, '.' ) == 0 )
                            {
                                valueIsFloat = false;
                            }

                            if ( stricmp( key, "Floor" ) == 0 )
                            {
                                kFLOOR = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "Gravity" ) == 0 )
                            {
                                kGRAVITY = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "BallHeight0" ) == 0 )
                            {
                                kMAX_HEIGHTS[ 0 ] = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "BallHeight1" ) == 0 )
                            {
                                kMAX_HEIGHTS[ 1 ] = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "BallHeight2" ) == 0 )
                            {
                                kMAX_HEIGHTS[ 2 ] = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "BallHeight3" ) == 0 )
                            {
                                kMAX_HEIGHTS[ 3 ] = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "BallHeight4" ) == 0 )
                            {
                                kMAX_HEIGHTS[ 4 ] = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "BallXSpeed" ) == 0 )
                            {
                                kBALL_X_SPEED = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "PlayerSpeed" ) == 0 )
                            {
                                kPLAYER_SPEED = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "SpearSpeed" ) == 0 )
                            {
                                kSPEAR_SPEED = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "SpearCeilingHoldTime" ) == 0 )
                            {
                                kSPEAR_HOLD_TIME = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "SpearBallHoldTime" ) == 0 )
                            {
                                kSPEAR_HOLD_TIME_AFTER_BALL_COLLISION = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "GodMode" ) == 0 )
                            {
                                kGOD_MODE = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                            }
                            else if ( stricmp( key, "BallScale0" ) == 0 )
                            {
                                kBALL_SCALES[ 0 ] = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                                ballScalesThatHaveChanged[ 0 ] = true;
                            }
                            else if ( stricmp( key, "BallScale1" ) == 0 )
                            {
                                kBALL_SCALES[ 1 ] = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                                ballScalesThatHaveChanged[ 1 ] = true;
                            }
                            else if ( stricmp( key, "BallScale2" ) == 0 )
                            {
                                kBALL_SCALES[ 2 ] = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                                ballScalesThatHaveChanged[ 2 ] = true;
                            }
                            else if ( stricmp( key, "BallScale3" ) == 0 )
                            {
                                kBALL_SCALES[ 3 ] = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                                ballScalesThatHaveChanged[ 3 ] = true;
                            }
                            else if ( stricmp( key, "BallScale4" ) == 0 )
                            {
                                kBALL_SCALES[ 4 ] = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                                ballScalesThatHaveChanged[ 4 ] = true;
                            }
                            else if ( stricmp( key, "PlayerScale" ) == 0 )
                            {
                                kPLAYER_SCALE = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                                spearScaleChanged = true;
                            }
                            else if ( stricmp( key, "SpearScale" ) == 0 )
                            {
                                kSPEAR_SCALE = ( float )( valueIsFloat ? atof( value ) : atoi( value ) );
                                playerScaleChanged = true;
                            }
                        }
                    }

                    linePos = 0;
                }
            }
        }

        delete [] pBuffer;

        // Balls.
        if ( m_pBallManager )
        {
            for ( std::vector< Ball * >::iterator it = m_pBallManager->m_Balls.begin( ); it != m_pBallManager->m_Balls.end( ); ++it )
            {
                Ball *pBall = *it;

                pBall->m_SpeedX = ( pBall->m_SpeedX > 0 ) ? kBALL_X_SPEED : -kBALL_X_SPEED;

                if ( ballScalesThatHaveChanged[ pBall->m_Size ] )
                {
                    float posX = pBall->m_pSprite->GetX( );
                    float posY = pBall->m_pSprite->GetY( );

                    delete pBall->m_pSprite;
                    pBall->m_pSprite = new Sprite(
                        kImageBalls[ pBall->m_Size ].Filename,
                        kImageBalls[ pBall->m_Size ].NumFrames,
                        0.f,
                        1.f,
                        false,
                        kBALL_SCALES[ pBall->m_Size ] );

                    pBall->m_pSprite->SetPosition( posX, posY );
                }
            }
        }

        // Player.
        if ( playerScaleChanged && m_pPlayer[ kPlayerAnimWalkLeft ] && m_pPlayer[ kPlayerAnimStand ] && m_pPlayer[ kPlayerAnimWalkRight ] )
        {
            float posX = (*m_ppPlayerCurrent)->GetX( );
            float width = (*m_ppPlayerCurrent)->GetWidth( );

            delete m_pPlayer[ kPlayerAnimWalkLeft ];
            delete m_pPlayer[ kPlayerAnimStand ];
            delete m_pPlayer[ kPlayerAnimWalkRight ];

            m_pPlayer[ kPlayerAnimWalkLeft ]    = new Sprite( kImagePlayerWalkLeft.Filename,    kImagePlayerWalkLeft.NumFrames, 0.05f, 1.f, true, kPLAYER_SCALE );
            m_pPlayer[ kPlayerAnimStand ]       = new Sprite( kImagePlayerIdle.Filename,        kImagePlayerIdle.NumFrames, 0.05f, 1.f, true, kPLAYER_SCALE );
            m_pPlayer[ kPlayerAnimWalkRight ]   = new Sprite( kImagePlayerWalkRight.Filename,   kImagePlayerWalkRight.NumFrames, 0.05f, 1.f, true, kPLAYER_SCALE );

            (*m_ppPlayerCurrent)->SetX( posX + ( 0.5f * width ) - ( 0.5f * (*m_ppPlayerCurrent)->GetWidth( ) ) );
            (*m_ppPlayerCurrent)->SetY( ( kFLOOR - (*m_ppPlayerCurrent)->GetHeight( ) * 0.5f ) - (*m_ppPlayerCurrent)->GetHeight( ) * 0.5f );
        }

        // Spear.
        if ( spearScaleChanged && m_pSpearManager )
        {
            for ( std::vector< Spear * >::iterator it = m_pSpearManager->m_Spears.begin( ); it != m_pSpearManager->m_Spears.end( ); ++it )
            {
                Spear *pSpear = *it;

                float posX = pSpear->m_pSprite->GetX( );
                float posY = pSpear->m_pSprite->GetY( );
                float width = pSpear->m_pSprite->GetWidth( );

                delete pSpear->m_pSprite;
                pSpear->m_pSprite = new Sprite( kImageSpear.Filename, kImageSpear.NumFrames, 0.05f, 1.f, true, kSPEAR_SCALE );

                pSpear->m_pSprite->SetX( posX + ( 0.5f * width ) - ( 0.5f * pSpear->m_pSprite->GetWidth( ) ) );
                pSpear->m_pSprite->SetY( posY );
            }
        }

        return true;
        #endif
    }
    bool PangGame::Initialise( )
    {
        #if defined(_PANG_DEBUG_MODE)
        if ( ! ReadSettings( ) )
            return false;
        #endif

        // Cache all images recources that might be deleted during game play.
        m_pSprites[ kLevelResourceImage_Ball0 ]     = new Sprite( kImageBalls[ 0 ].Filename, kImageBalls[ 0 ].NumFrames );
        m_pSprites[ kLevelResourceImage_Ball1 ]     = new Sprite( kImageBalls[ 1 ].Filename, kImageBalls[ 1 ].NumFrames );
        m_pSprites[ kLevelResourceImage_Ball2 ]     = new Sprite( kImageBalls[ 2 ].Filename, kImageBalls[ 2 ].NumFrames );
        m_pSprites[ kLevelResourceImage_Ball3 ]     = new Sprite( kImageBalls[ 3 ].Filename, kImageBalls[ 3 ].NumFrames );
        m_pSprites[ kLevelResourceImage_Ball4 ]     = new Sprite( kImageBalls[ 4 ].Filename, kImageBalls[ 4 ].NumFrames );
        m_pSprites[ kLevelResourceImage_Explosion ] = new Sprite( kImageExplosion.Filename, kImageExplosion.NumFrames );
        m_pSprites[ kLevelResourceImage_Spear ]     = new Sprite( kImageSpear.Filename, kImageSpear.NumFrames );

        m_pBallManager = new BallManager( );

        if ( !m_pBallManager->Initialise( ) )
            return false;

        m_pSpearManager = new SpearManager( );

        if ( !m_pSpearManager->Initialise( ) )
            return false;

        m_pExplosionManager = new ExplosionManager( );

        m_pBackgroundLayerManager = new BackgroundLayerManager;
        if ( !m_pBackgroundLayerManager->Initialise( ) )
            return false;
        if ( !m_pBackgroundLayerManager->AddLayer( kImageCloudsTop.Filename, 0, true, 20.f ) )
            return false;
        if ( !m_pBackgroundLayerManager->AddLayer( kImageCloudsMiddle2.Filename, 150, false, 15.f ) )
            return false;
        if ( !m_pBackgroundLayerManager->AddLayer( kImageCloudsMiddle1.Filename, 50, false, 10.f ) )
            return false;

        m_pBackground = new Sprite( kImageBackground.Filename, kImageBackground.NumFrames );
        m_pBackground->SetPosition( 0, 0 );

        m_pPlayer[ kPlayerAnimWalkLeft ]    = new Sprite( kImagePlayerWalkLeft.Filename,    kImagePlayerWalkLeft.NumFrames, 0.05f, 1.f, true, kPLAYER_SCALE );
        m_pPlayer[ kPlayerAnimStand ]       = new Sprite( kImagePlayerIdle.Filename,        kImagePlayerIdle.NumFrames, 0.05f, 1.f, true, kPLAYER_SCALE );
        m_pPlayer[ kPlayerAnimWalkRight ]   = new Sprite( kImagePlayerWalkRight.Filename,   kImagePlayerWalkRight.NumFrames, 0.05f, 1.f, true, kPLAYER_SCALE );
        (*m_ppPlayerCurrent)->SetX( ( m_pBackground->GetWidth( ) * 0.5f ) - (*m_ppPlayerCurrent)->GetWidth( ) * 0.5f );
        (*m_ppPlayerCurrent)->SetY( ( kFLOOR - (*m_ppPlayerCurrent)->GetHeight( ) * 0.5f ) - (*m_ppPlayerCurrent)->GetHeight( ) * 0.5f );

        m_Sfx[ kSoundResource_BallSpearHit ].Initialise( "sounds/Pang/hit.ogg", 5 );
        m_Sfx[ kSoundResource_Shoot ].Initialise( "sounds/Pang/shoot.ogg", 5 );

        m_pMusic = new Music;
        m_pMusic->loadMusic( "music/Pang/bg.ogg" );
        m_pMusic->setLooping( true );

        m_TimeLastFrame = SDL_GetTicks( );
        m_FrameTime     = m_TimeLastFrame;

        return true;
    }
    bool PangGame::Destroy( )
    {
        // Destroy all images resources that have been cached.
        for ( int i = 0; i < kLevelResourceImage_Num; ++i )
        {
            if ( m_pSprites[ i ] )
            {
                delete m_pSprites[ i ];
                m_pSprites[ i ] = NULL;
            }
        }

        if ( m_pBallManager )
        {
            m_pBallManager->Destroy( );
            delete m_pBallManager;
            m_pBallManager = NULL;
        }
        if ( m_pSpearManager )
        {
            m_pSpearManager->Destroy( );
            delete m_pSpearManager;
            m_pSpearManager = NULL;
        }
        if ( m_pExplosionManager )
        {
            delete m_pExplosionManager;
            m_pExplosionManager = NULL;
        }
        if ( m_pBackgroundLayerManager )
        {
            m_pBackgroundLayerManager->Destroy( );
            delete m_pBackgroundLayerManager;
            m_pBackgroundLayerManager = NULL;
        }
        if ( m_pBackground )
        {
            delete m_pBackground;
            m_pBackground = NULL;
        }
        for ( int i = 0; i < kPlayerAnimNum; ++i )
        {
            if ( m_pPlayer[ i ] )
            {
                delete m_pPlayer[ i ];
                m_pPlayer[ i ] = NULL;
            }
        }
        for ( int i = 0; i < kSoundResource_MAX; ++i )
        {
            m_Sfx[ i ].Destroy( );
        }
        if ( m_pMusic )
        {
            m_pMusic->freeAll( );
            delete m_pMusic;
            m_pMusic = NULL;
        }

        return true;
    }
    bool PangGame::IsLevelSuccessful( )
    {
        return m_LevelSuccessful;
    }
    bool PangGame::IsLevelFailed( )
    {
        return m_LevelFailed;
    }
    void PangGame::SetLevelComplete( bool successful )
    {
        if ( successful )
            m_LevelSuccessful = true;
        else
            m_LevelFailed = true;
    }
    bool PangGame::StartLevel( int levelNumber )
    {
        m_NumFramesElapsedSinceLevelStart = 0;

        int levelLayout = levelNumber;

        if ( levelLayout >= 20 )
        {
            levelLayout = 10 + ( levelLayout % 10 );
        }

        int multiplier = 1;
        if ( levelNumber > levelLayout )
            multiplier = ( levelNumber - levelLayout ) / 10;

        kBALL_X_SPEED = kBALL_X_SPEED_INITIAL * ( 1 + ( multiplier * 0.25f ) );
        kGRAVITY = kGRAVITY_INITIAL * ( 1 + ( multiplier * 0.25f ) );

        if ( levelLayout == 0 )
        {
            m_pBallManager->Add( 400, 300, 3, Ball::kDirectionRight );
        }
        else if ( levelLayout == 1 )
        {
            m_pBallManager->Add( 100, 300, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 700, 300, 4, Ball::kDirectionLeft );
        }
        else if ( levelLayout == 2 )
        {
            m_pBallManager->Add( 100, 300, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 200, 300, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 600, 300, 4, Ball::kDirectionLeft );
            m_pBallManager->Add( 700, 300, 4, Ball::kDirectionLeft );
        }
        else if ( levelLayout == 3 )
        {
            m_pBallManager->Add( 100, 300, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 150, 250, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 200, 300, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 600, 300, 4, Ball::kDirectionLeft );
            m_pBallManager->Add( 650, 250, 4, Ball::kDirectionLeft );
            m_pBallManager->Add( 700, 300, 4, Ball::kDirectionLeft );
        }
        else if ( levelLayout == 4 )
        {
            m_pBallManager->Add( 100, 250, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 700, 250, 3, Ball::kDirectionLeft );
        }
        else if ( levelLayout == 5 )
        {
            m_pBallManager->Add( 100, 250, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 200, 250, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 600, 250, 3, Ball::kDirectionLeft );
            m_pBallManager->Add( 700, 250, 3, Ball::kDirectionLeft );
        }
        else if ( levelLayout == 6 )
        {
            m_pBallManager->Add( 100, 250, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 150, 200, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 200, 250, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 600, 250, 3, Ball::kDirectionLeft );
            m_pBallManager->Add( 650, 200, 3, Ball::kDirectionLeft );
            m_pBallManager->Add( 700, 250, 3, Ball::kDirectionLeft );
        }
        else if ( levelLayout == 7 )
        {
            m_pBallManager->Add( 400, 200, 1, Ball::kDirectionRight );
        }
        else if ( levelLayout == 8 )
        {
            m_pBallManager->Add( 200, 200, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 300, 200, 2, Ball::kDirectionRight );
            m_pBallManager->Add( 500, 200, 2, Ball::kDirectionRight );
            m_pBallManager->Add( 600, 200, 4, Ball::kDirectionRight );
        }
        else if ( levelLayout == 9 )
        {
            m_pBallManager->Add( 200, 200, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 300, 200, 2, Ball::kDirectionRight );
            m_pBallManager->Add( 500, 200, 2, Ball::kDirectionRight );
            m_pBallManager->Add( 600, 200, 3, Ball::kDirectionRight );
        }
        else if ( levelLayout == 10 )
        {
            m_pBallManager->Add( 300, 200, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 400, 200, 1, Ball::kDirectionRight );
            m_pBallManager->Add( 500, 200, 3, Ball::kDirectionRight );
        }
        else if ( levelLayout == 11 )
        {
            m_pBallManager->Add( 200, 200, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 300, 200, 2, Ball::kDirectionRight );
            m_pBallManager->Add( 400, 200, 1, Ball::kDirectionRight );
            m_pBallManager->Add( 500, 200, 2, Ball::kDirectionRight );
            m_pBallManager->Add( 600, 200, 3, Ball::kDirectionRight );
        }
        else if ( levelLayout == 12 )
        {
            m_pBallManager->Add( 300, 200, 2, Ball::kDirectionRight );
            m_pBallManager->Add( 500, 200, 2, Ball::kDirectionRight );
        }
        else if ( levelLayout == 13 )
        {
            m_pBallManager->Add( 100, 100, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 125, 140, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 150, 180, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 175, 220, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 200, 260, 3, Ball::kDirectionRight );

            m_pBallManager->Add( 700, 100, 3, Ball::kDirectionLeft );
            m_pBallManager->Add( 675, 140, 3, Ball::kDirectionLeft );
            m_pBallManager->Add( 650, 180, 3, Ball::kDirectionLeft );
            m_pBallManager->Add( 625, 220, 3, Ball::kDirectionLeft );
            m_pBallManager->Add( 600, 260, 3, Ball::kDirectionLeft );
        }
        else if ( levelLayout == 14 )
        {
            m_pBallManager->Add( 100, 100, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 125, 140, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 150, 180, 2, Ball::kDirectionRight );
            m_pBallManager->Add( 175, 220, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 200, 260, 3, Ball::kDirectionRight );

            m_pBallManager->Add( 700, 100, 3, Ball::kDirectionLeft );
            m_pBallManager->Add( 675, 140, 3, Ball::kDirectionLeft );
            m_pBallManager->Add( 650, 180, 2, Ball::kDirectionLeft );
            m_pBallManager->Add( 625, 220, 3, Ball::kDirectionLeft );
            m_pBallManager->Add( 600, 260, 3, Ball::kDirectionLeft );
        }
        else if ( levelLayout == 15 )
        {
            m_pBallManager->Add( 350, 200, 1, Ball::kDirectionRight );
            m_pBallManager->Add( 450, 200, 1, Ball::kDirectionRight );
        }
        else if ( levelLayout == 16 )
        {
            m_pBallManager->Add( 300, 220, 2, Ball::kDirectionRight );
            m_pBallManager->Add( 350, 200, 1, Ball::kDirectionRight );
            m_pBallManager->Add( 450, 200, 1, Ball::kDirectionRight );
            m_pBallManager->Add( 500, 220, 2, Ball::kDirectionRight );
        }
        else if ( levelLayout == 17 )
        {
            m_pBallManager->Add( 400, 200, 0, Ball::kDirectionRight );
        }
        else if ( levelLayout == 18 )
        {
            m_pBallManager->Add( 200, 200, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 250, 130, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 300, 200, 2, Ball::kDirectionRight );
            m_pBallManager->Add( 350, 130, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 400, 200, 1, Ball::kDirectionRight );
            m_pBallManager->Add( 450, 130, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 500, 200, 2, Ball::kDirectionRight );
            m_pBallManager->Add( 550, 130, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 600, 200, 3, Ball::kDirectionRight );
        }
        else if ( levelLayout == 19 )
        {
            m_pBallManager->Add( 200, 200, 2, Ball::kDirectionRight );
            m_pBallManager->Add( 250, 130, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 350, 130, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 400, 200, 0, Ball::kDirectionRight );
            m_pBallManager->Add( 450, 130, 3, Ball::kDirectionRight );
            m_pBallManager->Add( 550, 130, 4, Ball::kDirectionRight );
            m_pBallManager->Add( 600, 200, 2, Ball::kDirectionRight );
        }

        return true;
    }
    bool PangGame::Update( SimpleJoy * input )
    {
        #if defined(_PANG_DEBUG_MODE)
        ReadSettings( );
        #endif

        if ( g_Frozen )
            return true;

        if ( m_NumFramesElapsedSinceLevelStart > 0 && ! m_pMusic->isPlaying( ) )
        {
            m_pMusic->play( );
        }

        m_NumFramesElapsedSinceLevelStart++;

        // Sort out framne time.
        float thisFrameTime = SDL_GetTicks( );
        m_FrameTime = ( thisFrameTime - m_TimeLastFrame ) / 1000.f;
        m_TimeLastFrame = thisFrameTime;

        // When capturing FMV...
        //m_FrameTime = 1/30.f;

        // Sort out controls.
        Controls::Update( input );

        // Set player position based on left/right input.
        if ( ( Controls::IsLeftPressed( ) && Controls::IsRightPressed( ) ) || ( !Controls::IsLeftPressed( ) && !Controls::IsRightPressed( ) ) )
        {
            m_pPlayer[ kPlayerAnimStand ]->SetPosition( (*m_ppPlayerCurrent)->GetX( ), (*m_ppPlayerCurrent)->GetY( ) );
            m_ppPlayerCurrent = &m_pPlayer[ kPlayerAnimStand ];
        }
        else if ( Controls::IsLeftPressed( ) )
        {
            m_pPlayer[ kPlayerAnimWalkLeft ]->SetPosition( (*m_ppPlayerCurrent)->GetX( ), (*m_ppPlayerCurrent)->GetY( ) );
            m_ppPlayerCurrent = &m_pPlayer[ kPlayerAnimWalkLeft ];
            (*m_ppPlayerCurrent)->SetX( (*m_ppPlayerCurrent)->GetX( ) - kPLAYER_SPEED * m_FrameTime );
            if ( (*m_ppPlayerCurrent)->GetX( ) < 0.f )
                (*m_ppPlayerCurrent)->SetX( 0 );
        }
        else if ( Controls::IsRightPressed( ) )
        {
            m_pPlayer[ kPlayerAnimWalkRight ]->SetPosition( (*m_ppPlayerCurrent)->GetX( ), (*m_ppPlayerCurrent)->GetY( ) );
            m_ppPlayerCurrent = &m_pPlayer[ kPlayerAnimWalkRight ];
            (*m_ppPlayerCurrent)->SetX( (*m_ppPlayerCurrent)->GetX( ) + kPLAYER_SPEED * m_FrameTime );
            if ( (*m_ppPlayerCurrent)->GetX( ) + (*m_ppPlayerCurrent)->GetWidth( ) > kSCREEN_WIDTH )
                (*m_ppPlayerCurrent)->SetX( kSCREEN_WIDTH - (*m_ppPlayerCurrent)->GetWidth( ) );
        }

        if ( Controls::IsUpHit( ) )
        {
            if ( m_pSpearManager->CanAddSpear( ) )
            {
                m_Sfx[ kSoundResource_Shoot ].Play( );
                m_pSpearManager->Add( (*m_ppPlayerCurrent)->GetX( ) + 0.5f * (*m_ppPlayerCurrent)->GetWidth( ), (*m_ppPlayerCurrent)->GetY( ) - 0.5f * (*m_ppPlayerCurrent)->GetHeight( ) );
            }
        }

        // Update player sprite animation.
        (*m_ppPlayerCurrent)->Update( m_FrameTime );

        // Update managers;
        m_pBallManager->UpdateAll( m_FrameTime );
        m_pSpearManager->UpdateAll( m_FrameTime );
        m_pExplosionManager->UpdateAll( m_FrameTime );
        m_pBackgroundLayerManager->Update( m_FrameTime );

        // Collision detection.
        std::vector< Ball* > toAdd;

        for ( std::vector< Ball* >::iterator ballIt = m_pBallManager->m_Balls.begin( ); ballIt != m_pBallManager->m_Balls.end( ); )
        {
            Ball *pBall = *ballIt;
            bool ballHit = false;
            for ( std::vector< Spear* >::iterator spearIt = m_pSpearManager->m_Spears.begin( ); spearIt != m_pSpearManager->m_Spears.end( ); )
            {
                Spear *pSpear = *spearIt;

                if ( pSpear->m_State == Spear::kState_Active )
                {
                    if ( pBall->m_pSprite->HasCollidedWith( pSpear->m_pSprite ) )
                    {
                        if ( g_FreezeOnBallSpearCollision )
                        {
                            g_Frozen = true;
                        }
                        else
                        {
                            ballHit = true;
                            pSpear->m_State = Spear::kState_HitBall;
                            pSpear->m_HitTime = kSPEAR_HOLD_TIME_AFTER_BALL_COLLISION;
                            m_Sfx[ kSoundResource_BallSpearHit ].Play( );
                            break;
                        }
                    }
                }
                ++spearIt;
            }
            if ( ballHit == false )
            {
                ballIt++;
            }
            else
            {
                float ballCenterX = pBall->m_pSprite->GetX( ) + 0.5f * pBall->m_pSprite->GetWidth( );
                float ballCenterY = pBall->m_pSprite->GetY( ) + 0.5f * pBall->m_pSprite->GetHeight( );
                m_pExplosionManager->Add( ballCenterX, ballCenterY );
                if ( pBall->m_Size < 4 )
                {
                    toAdd.push_back( new Ball(  ballCenterX,
                                                ballCenterY,
                                                pBall->m_Size + 1, Ball::kDirectionLeft ) );
                    toAdd.push_back( new Ball(  pBall->m_pSprite->GetX( ) + 0.5f * pBall->m_pSprite->GetWidth( ),
                                                pBall->m_pSprite->GetY( ) + 0.5f * pBall->m_pSprite->GetHeight( ),
                                                pBall->m_Size + 1, Ball::kDirectionRight ) );
                }
                delete *ballIt;
                ballIt = m_pBallManager->m_Balls.erase( ballIt );
                continue;
            }
            if ( !kGOD_MODE && pBall->m_pSprite->HasCollidedWith( (*m_ppPlayerCurrent) ) )
            {
                if ( g_FreezeOnPlayerBallCollision )
                {
                    g_Frozen = true;
                }
                else
                {
                    SetLevelComplete( false );
                }
            }
        }
        for ( std::vector< Ball* >::iterator ballIt = toAdd.begin( ); ballIt != toAdd.end( ); ++ballIt )
        {
            m_pBallManager->Add( *ballIt );
        }

        // Check for level compelte.
        if ( m_pBallManager->m_Balls.size( ) == 0 )
        {
            SetLevelComplete( true );
        }

        return true;
    }
    #ifdef PENJIN_SDL
    bool PangGame::Render(SDL_Surface *screen )
    #else
    bool PangGame::Render( )
    #endif
    {
        Sprite::ClassInit( GET_SCREEN( ) );

        GFX::clearScreen( GET_SCREEN( ) );

        m_pBackground->Render( );

        m_pBackgroundLayerManager->Render( m_FrameTime );

        m_pSpearManager->RenderAll( );

        (*m_ppPlayerCurrent)->Render( );

        m_pBallManager->RenderAll( );

        m_pExplosionManager->RenderAll( );

        return true;
    }
    // --------------------------------------------------------------

};  // namespace PangMiniGame

// --------------------------------------------------------------
// StatePang.
// --------------------------------------------------------------
StatePang::StatePang( )
{
    m_pPangGame     = NULL;
}

void StatePang::init( )
{
    buttonSheet.loadFrames("images/ButtonPrompter/ButtonsSheet.png",10,2);
    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(BLACK);

    m_pPangGame = new PangMiniGame::PangGame( );
    m_pPangGame->Initialise( );
    m_pPangGame->StartLevel( GetRequestedLevelNumber( ) );
}

#ifdef PENJIN_SDL
void StatePang::render( SDL_Surface *screen )
#else
void StatePang::render( )
#endif
{
    m_pPangGame->Render( GET_SCREEN( ) );
}
#ifdef PENJIN_SDL
void StatePang::pauseScreen(SDL_Surface* screen)
{
    // Pause screen
    pauseSymbol(screen);
    pauseText.setPosition(50,180);
    pauseText.print(screen, "Burst those balls, but don't let them hit you!");
    pauseText.setPosition(50,220);
    pauseText.print(screen, "Press     or     to shoot your spear!");
    buttonSheet.setCurrentFrame(10);
    buttonSheet.setPosition(130,220);
    buttonSheet.render(screen);
    buttonSheet.setCurrentFrame(16);
    buttonSheet.setPosition(210,220);
    buttonSheet.render(screen);
}
#else
void StatePang::pauseScreen()
{
    // Pause screen
    pauseSymbol();
    pauseText.setPosition(50,180);
    pauseText.print("Burst those balls, but don't let them hit you!");
    pauseText.setPosition(50,220);
    pauseText.print("Press     or     to shoot your spear!");
    buttonSheet.setCurrentFrame(10);
    buttonSheet.setPosition(130,220);
    buttonSheet.render();
    buttonSheet.setCurrentFrame(16);
    buttonSheet.setPosition(210,220);
    buttonSheet.render();
}
#endif
void StatePang::update( )
{
    // Pump game.
    m_pPangGame->Update( input );

    if ( m_pPangGame->IsLevelSuccessful( ) )
    {
        MiniGameComplete( true );
    }
    else if ( m_pPangGame->IsLevelFailed( ) )
    {
        MiniGameComplete( false );
    }
}

StatePang::~StatePang( )
{
    if ( m_pPangGame )
    {
        m_pPangGame->Destroy( );
        delete m_pPangGame;
    }
}

int StatePang::GetRequestedLevelNumber( )
{
    return variables[ 2 ].getInt( );
}

void StatePang::MiniGameComplete( bool success )
{
    variables[ 0 ].setInt( success ? 1 : 0 );
    setNextState( STATE_MAIN );
}
// --------------------------------------------------------------
