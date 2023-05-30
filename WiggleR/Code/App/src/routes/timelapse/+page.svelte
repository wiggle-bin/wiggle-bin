<script lang="ts">
	import { start, end, showVideo, showContours } from '../../store/store';

	let startTime = '';
	let endTime = '';
	let videoActive: null | boolean = false;
	let contoursActive: null | boolean = false;

	start.subscribe((time) => (startTime = time));
	end.subscribe((time) => (endTime = time));
	showVideo.subscribe((show) => (videoActive = Boolean(show)));
	showContours.subscribe((show) => (contoursActive = Boolean(show)));
</script>

{#key [startTime, endTime, videoActive, contoursActive]}
	<video width="100%" controls>
		<source
			src={`http://localhost:8000/video?startTime=${startTime}&endTime=${endTime}&showVideo=${videoActive}&showContours=${contoursActive}`}
			type="video/mp4"
		/>
	</video>
{/key}
