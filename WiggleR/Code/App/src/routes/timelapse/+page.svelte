<script lang="ts">
	import { start, end, showThresh, showContours } from '../../store/store';

	let startTime = '';
	let endTime = '';
	let threshActive: null | boolean = false;
	let contoursActive: null | boolean = false;

	start.subscribe((time) => (startTime = time));
	end.subscribe((time) => (endTime = time));
	showThresh.subscribe((show) => (threshActive = Boolean(show)));
	showContours.subscribe((show) => (contoursActive = Boolean(show)));
</script>

{#key [startTime, endTime, threshActive, contoursActive]}
	<video width="100%" controls>
		<source
			src={`http://localhost:8000/video?startTime=${startTime}&endTime=${endTime}&showThresh=${threshActive}&showContours=${contoursActive}`}
			type="video/mp4"
		/>
	</video>
{/key}
