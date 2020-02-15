# Note: This repo has been deleted, and this information is listed for archival purposes only

## ListenBrainz 2

[![GitHub stars](https://img.shields.io/github/stars/marc2k3/foo_listenbrainz2?style=social)](https://github.com/marc2k3/foo_listenbrainz2)

### Download

[https://github.com/marc2k3/foo_listenbrainz2/releases](https://github.com/marc2k3/foo_listenbrainz2/releases)

### Minimum requirements

`Windows 7` and [foobar2000](https://foobar2000.org) `v1.5`.

### Installation

[https://wiki.hydrogenaud.io/index.php?title=Foobar2000:How_to_install_a_component](https://wiki.hydrogenaud.io/index.php?title=Foobar2000:How_to_install_a_component)

### What is ListenBrainz?

[https://listenbrainz.org](https://listenbrainz.org)

### Why foo_listenbrainz2?

Somebody else already beat me with the original name here: [https://github.com/medvid/foo_listenbrainz](https://github.com/medvid/foo_listenbrainz)

I decided to write my own because the above version blocks the main thread and freezes the UI when submitting. Also, my version submits artist/recording [MBID](https://musicbrainz.org/doc/MusicBrainz_Identifier)s if present in your files. This makes artist/title links on your recent listens page clickable and will lead you to the respective pages on [musicbrainz.org](https://musicbrainz.org)

Note that [foobar2000](https://foobar2000.org) does not support reading `UFID` tags that are written to MP3s by [MusicBrainz Picard](https://picard.musicbrainz.org/).

### Preferences

Enter your [ListenBrainz token](https://listenbrainz.org/profile) and check the other options under `File>Preferences>Tools>ListenBrainz 2`.

![Preferences](images/listenbrainz_preferences.png)

Now just listen to your music and watch the `Console` for server responses and/or check your recent listens page. Like [Last.fm](https://last.fm), you must listen to at least half the track or 4 minutes - whichever comes first.

As of `v1.1.0-Beta.2`, the following tags are submitted:

Required (tracks without them are silently ignored)
```
ARTIST
TITLE
```

Sent if present
```
ALBUM ARTIST
ALBUM
DATE
TRACKNUMBER
TOTALTRACKS
DISCNUMBER
TOTALDISCS
ISRC
MUSICBRAINZ_ARTISTID / MUSICBRAINZ ARTIST ID
MUSICBRAINZ_TRACKID / MUSICBRAINZ TRACK ID
MUSICBRAINZ_ALBUMID / MUSICBRAINZ ALBUM ID
MUSICBRAINZ_RELEASEGROUPID / MUSICBRAINZ RELEASE GROUP ID
MUSICBRAINZ_RELEASETRACKID / MUSICBRAINZ RELEASE TRACK ID
```

If any tags are multi-value, only the first value is sent.

### Caching of failed submissions

This feature was added in `v1.1.0-Beta.1` and happens automatically. You don't need to do anything to trigger it. If you're offline/[ListenBrainz](https://listenbrainz.org) is down, the `Console` will report each time a `listen` is cached and the next time you submit a successful `listen`, an attempt will be made to submit all cached entries.

### Limitations

Currently there is no support for streams or tracks of an unknown length.
