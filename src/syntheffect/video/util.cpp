namespace syntheffect {
    namespace video {
        void seek(ofVideoPlayer player, int relative_frames) {
            int current = player.getCurrentFrame();
            // Will be negative if we haven't done an update yet
            if (current < 0) {
                return;
            }

            int seek_to = relative_frames + current;
            if (seek_to < 0) {
                seek_to += player.getTotalNumFrames();
            }

            player.setFrame(seek_to % player.getTotalNumFrames());
        }
    }
}
