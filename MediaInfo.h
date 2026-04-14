// The MIT License( MIT )
//
// Copyright( c ) 2026 Towel 42 Development, LLC and Scott Aron Bloom
// SPDX-License-Identifier : MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef __MEDIAINFO_H
#define __MEDIAINFO_H

#include "Towel42MediaUtilsExport.h"

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QFileInfo>
#include <QObject>
#include <QMutex>
#include <unordered_map>
#include <map>
#include <memory>
#include <set>

class QFileInfo;
namespace MediaInfoDLL
{
    class MediaInfo;
}

namespace NTowel42MediaUtils
{
    enum class EStreamType
    {
        eGeneral,
        eVideo,
        eAudio,
        eText,
        eOther,
        eImage,
        eMenu
    };

    QString toString( EStreamType streamType );
}

namespace std
{
    template<>
    struct hash< NTowel42MediaUtils::EStreamType >
    {
        std::size_t operator()( const NTowel42MediaUtils::EStreamType &ii ) const
        {
            auto tmp = static_cast< int >( ii );
            return std::hash< int >()( tmp );
        }
    };
}

namespace NTowel42MediaUtils
{
    class CFFMpegFormats;
    enum class EMediaTags
    {
        eFileName,
        eTitle,
        eLengthMS,
        eLengthS,
        eLength,
        eDate,
        eComment,
        eBPM,
        eArtist,
        eComposer,
        eGenre,
        eTrack,
        eAlbum,
        eAlbumArtist,
        eDiscnumber,
        eAspectRatio,
        eDisplayAspectRatio,
        eWidth,
        eHeight,
        eResolution,
        eVideoCodec,
        eFirstVideoCodec,
        eAllVideoCodecs,
        eAudioCodec,
        eAudioCodecDisp,
        eFirstAudioCodec,
        eFirstAudioCodecDisp,
        eAllAudioCodecs,
        eAllAudioCodecsDisp,
        eVideoBitrate,   // if not found falls back to overall bitrate
        eVideoBitrateString,
        eHDRInfo,
        eOverAllBitrate,
        eOverAllBitrateString,
        eBitsPerPixel,
        eBitDepth,
        eFrameCount,
        eFrameRate,
        eFrameRateNum,
        eFrameRateDen,
        eScanType,
        eAudioChannelCount,
        eTotalAudioBitrate,
        eTotalAudioBitrateString,
        eAudioBitrate,
        eAudioBitrateString,
        eAudioSampleRate,
        eAudioSampleRateString,
        eNumVideoStreams,
        eNumAudioStreams,
        eNumSubtitleStreams,
        eSubtitleLanguage,
        eFirstSubtitleLanguage,
        eAllSubtitleLanguages,
        eAllSubtitleDispString,
        eSubtitleCodec,
        eFirstSubtitleCodec,
        eAllSubtitleCodecs,
        eDefaultVideoStream,
        eDefaultAudioStream,
        eDefaultSubtitleStream,
        eLastTag
    };
    TOWEL42_MEDIAUTILS_EXPORT QString displayName( EMediaTags tag );
    TOWEL42_MEDIAUTILS_EXPORT EMediaTags fromDisplayName( const QString &tag );
    TOWEL42_MEDIAUTILS_EXPORT QString getMKVEditName( EMediaTags tag );
    TOWEL42_MEDIAUTILS_EXPORT bool isSettableTag( EMediaTags tag );
    TOWEL42_MEDIAUTILS_EXPORT QString mediaInfoTagName( EMediaTags tag );
    TOWEL42_MEDIAUTILS_EXPORT std::list< QString > toStringList( const std::list< EMediaTags > &keys );

    TOWEL42_MEDIAUTILS_EXPORT EMediaTags fromTagName( const QString &tag );

}

namespace std
{
    template<>
    struct hash< NTowel42MediaUtils::EMediaTags >
    {
        std::size_t operator()( const NTowel42MediaUtils::EMediaTags &ii ) const
        {
            auto tmp = static_cast< int >( ii );
            return std::hash< int >()( tmp );
        }
    };
}

namespace NTowel42MediaUtils
{
    enum class EMediaTags;
    using TMediaTagMap = std::unordered_map< EMediaTags, QVariant >;

    class CStreamData;
    class CMediaInfoImpl;
    struct TOWEL42_MEDIAUTILS_EXPORT SResolutionInfo
    {
        std::pair< int, int > fResolution{ 0, 0 };
        bool fInterlaced{ false };
        double fFPS{ 0.0 };
        uint64_t fNumFrames{ 0 };
        uint64_t fNumSeconds{ 0 };
        uint32_t fBitsPerPixel{ 0 };

        uint64_t idealBitrate() const;
        bool isGreaterThan4kResolution( double threshold = 0.2 ) const;
        bool is4kResolution( double threshold = 0.2 ) const;
        bool isGreaterThanHDResolution( double threshold = 0.0 ) const;
        bool isHDResolution( double threshold = 0.2 ) const;
        bool isSubHDResolution( double threshold = 0.2 ) const;

        bool isResolution( const std::pair< int, int > &target, double threshold = 0.2 ) const;   // within +/- threshold
        bool isGreaterThanResolution( const std::pair< int, int > &min, double threshold = 0.2 ) const;   // greater than + threshold
        bool isLessThanResolution( const std::pair< int, int > &max, double threshold = 0.2 ) const;   // less than - threshold
    private:
    };

    class TOWEL42_MEDIAUTILS_EXPORT CMediaInfo : public QObject
    {
        Q_OBJECT;
        friend class CMediaInfoMgr;

    private:
        struct SPrivate
        {
        };

    public:
        CMediaInfo( const SPrivate & pri );
        CMediaInfo( const QString &fileName, bool delayLoad, const SPrivate &pri );
        CMediaInfo( const QFileInfo &fi, bool delayLoad, const SPrivate &pri );

        CMediaInfo( const QString &fileName );
        CMediaInfo( const QFileInfo &fi );

        static std::shared_ptr< CMediaInfo > create();
        static std::shared_ptr< CMediaInfo > create( const QString &fileName, bool delayLoad );
        static std::shared_ptr< CMediaInfo > create( const QFileInfo &fi, bool delayLoad );

        ~CMediaInfo();

        static void setFFProbeEXE( const QString &path );
        static QString ffprobeEXE();

        static SResolutionInfo k8KResolution();
        static SResolutionInfo k4KResolution();
        static SResolutionInfo k1080pResolution();
        static SResolutionInfo k1080iResolution();
        static SResolutionInfo k720Resolution();
        static SResolutionInfo k480Resolution();

        QString statusText() const;

        bool load();
        bool aOK() const;
        bool isQueued() const;

        QString fileName() const;
        QString version() const;

        bool isDefaultAudioCodec( const QString &checkCodecName, CFFMpegFormats *ffmpegFormats ) const;
        bool isDefaultAudioCodecAAC( CFFMpegFormats *ffmpegFormats, int maxNumChannels ) const;
        bool hasVideoCodec( const QString &checkCodecName, CFFMpegFormats *ffmpegFormats ) const;
        bool isContainerFormat( const QString &formatName, CFFMpegFormats *ffmpegFormats ) const;
        bool isCodec( const QString &checkCodecName, const QString &mediaCodecName, CFFMpegFormats *ffmpegFormats );
        bool isHEVCCodec( QString mediaCodecName, CFFMpegFormats *ffmpegFormats );

        TMediaTagMap getSettableMediaTags() const;
        TMediaTagMap getMediaTags( const std::list< EMediaTags > &tags = {} ) const;

        QString getMediaTag( EMediaTags tag ) const;
        QString getMediaTag( size_t streamNum, EMediaTags tag ) const;

        uint64_t getBitRate( EStreamType streamType, size_t streamNum ) const;

        uint64_t getOverallBitRate() const;
        uint64_t getVideoBitRate() const;
        uint64_t getDefaultAudioBitRate() const;
        uint64_t getUncompressedVideoBitRate() const;   // calculated value = width * height * bits per pixel * frames per second
        uint64_t getUncompressedOverallBitRate() const;   // calculated value = width * height * bits per pixel * frames per second + audio size per stream

        uint64_t getBitRate() const;

        SResolutionInfo getResolutionInfo() const;

        std::pair< int, int > getResolution() const;
        uint64_t getNumberOfFrames() const;
        double getFrameRate() const;
        uint32_t getBitsPerPixel() const;
        bool getInterlaced() const;

        // threshold is on total pixels per frame
        bool isGreaterThan4kResolution( double threshold = 0.2 ) const;
        bool is4kResolution( double threshold = 0.2 ) const;
        bool isGreaterThanHDResolution( double threshold = 0.0 ) const;
        bool isHDResolution( double threshold = 0.2 ) const;
        bool isSubHDResolution( double threshold = 0.2 ) const;

        bool isResolution( const std::pair< int, int > &target, double threshold = 0.2 ) const;

        int64_t getNumberOfSeconds() const;
        int64_t getNumberOfMSecs() const;

        static int64_t getNumberOfSeconds( const QString &fileName );
        static int64_t getNumberOfMSecs( const QString &fileName );

        static QString getMediaTag( const QString &fileName, EMediaTags tag );
        static TMediaTagMap getMediaTags( const QString &path, const std::list< EMediaTags > &tags );

        int numAudioStreams() const;
        int numVideoStreams() const;
        int numSubtitleStreams() const;

        int defaultAudioStream() const;
        int defaultVideoStream() const;
        int defaultSubtitleStream() const;

        int audioChannelCount( size_t streamNum = -1 ) const;

        QStringList allSubtitleCodecs() const;

    private:
        [[nodiscard]] bool queueLoad();
        std::shared_ptr< CMediaInfoImpl > fImpl;
    };

    class TOWEL42_MEDIAUTILS_EXPORT CMediaInfoMgr : public QObject
    {
        CMediaInfoMgr();
        Q_OBJECT;

    public:
        static CMediaInfoMgr *instance();
        virtual ~CMediaInfoMgr() override {}

        std::shared_ptr< CMediaInfo > getMediaInfo( const QString &fileName );   // creates a mediainfo, then loads it in a background thread, then CMediaInfo will emit when its finished
        std::shared_ptr< CMediaInfo > getMediaInfo( const QFileInfo &fi );

        bool isMediaCached( const QString &fileName ) const;
        bool isMediaCached( const QFileInfo &fi ) const;

        void mediaLoaded( const QString &fileName );
        void mediaQueued( const QString &fileName );
        void mediaFinished( const QString &fileName, bool success );

        bool isProcessing();

    private Q_SLOTS:
        void slotMediaInfoQueueTimout();
    Q_SIGNALS:
        void sigMediaLoaded( const QString &fileName );
        void sigMediaQueued( const QString &fileName );
        void sigMediaFinished( const QString &fileName, bool success );
        void sigStatusMessage( const QString &msg, bool debugData );
        void sigFinishedProcessingMedia();

    private:
        QTimer *fMediaInfoQueueTimer{ nullptr };
        std::optional< int > fNumQueuedEmpty;

        QMutex fMutex;
        std::unordered_map< QString, std::shared_ptr< CMediaInfo > > fQueuedMediaInfo;   // store here while media is loading
    };
}

#endif
